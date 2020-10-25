#include <stdio.h>
#include <string.h>
#include <sotc/sotc.h>
#include <sotc/model.h>
#include <json.h>


int sotc_add_state(struct sotc_region *region, const char *name,
                    struct sotc_state **out)
{
    struct sotc_state *state = malloc(sizeof(struct sotc_state));

    if (!state)
        return -SOTC_ERR_MEM;

    memset(state, 0, sizeof(*state));

    (*out) = state;

    state->name = strdup(name);

    if (region->last_state)
        region->last_state->next = state;
    if (!region->state)
        region->state = state;

    region->last_state = state;

    return SOTC_OK;
}

int sotc_state_append_region(struct sotc_state *state, struct sotc_region *r)
{
    if (state)
    {
        L_DEBUG("Belongs to %s", state->name);
        /* Place region last in the list of regions for this state */
        if (!state->regions)
        {
            state->regions = r;
        }

        if (!state->last_region)
        {
            state->last_region = r;
        }
        else
        {
            state->last_region->next = r;
            state->last_region = r;
        }
    }

    return SOTC_OK;
}

int sotc_state_set_size(struct sotc_state *s, double x, double y)
{
    s->w = x;
    s->h = y;
    return SOTC_OK;
}

int sotc_state_set_xy(struct sotc_state *s, double x, double y)
{
    s->x = x;
    s->y = y;
    return SOTC_OK;
}

int sotc_state_get_size(struct sotc_state *s, double *x, double *y)
{
    (*x) = s->w;
    (*y) = s->h;
    return SOTC_OK;
}

int sotc_state_get_xy(struct sotc_state *s, double *x, double *y)
{
    (*x) = s->x;
    (*y) = s->y;
    return SOTC_OK;
}

static int serialize_action_list(struct sotc_action_ref *list,
                                 json_object *output)
{
    json_object *action;
    char uuid_str[37];
    struct sotc_action_ref *tmp = list;

    while (tmp) {
        uuid_unparse(tmp->act->id, uuid_str);
        action = json_object_new_object();
        L_DEBUG("adding entry ref!");
        json_object_object_add(action, "id", json_object_new_string(uuid_str));
        json_object_array_add(output, action);
        tmp = tmp->next;
    }

    return SOTC_OK;
}

/* Translate the internal structure to json */
int sotc_state_serialize(struct sotc_state *state, json_object *region,
                        json_object **out)
{
    int rc;
    json_object *j_state = json_object_new_object();
    json_object *j_name = json_object_new_string(state->name);
    json_object *j_kind = json_object_new_string("state");
    json_object *j_region = json_object_new_array();

    json_object *j_entries = json_object_new_array();
    json_object *j_exits = json_object_new_array();

    json_object_object_add(j_state, "name", j_name);
    json_object_object_add(j_state, "kind", j_kind);

    json_object_object_add(j_state, "width",
                json_object_new_double(state->w));

    json_object_object_add(j_state, "height",
                json_object_new_double(state->h));

    json_object_object_add(j_state, "x",
                json_object_new_double(state->x));
    json_object_object_add(j_state, "y",
                json_object_new_double(state->y));

    rc = serialize_action_list(state->entries, j_entries);
    if (rc != SOTC_OK)
        return rc;

    rc = serialize_action_list(state->exits, j_exits);
    if (rc != SOTC_OK)
        return rc;

    json_object_object_add(j_state, "entries", j_entries);
    json_object_object_add(j_state, "exits", j_exits);

    json_object_object_add(j_state, "region", j_region);

    (*out) = j_state;

    json_object *j_region_state_array;

    if (!json_object_object_get_ex(region, "states", &j_region_state_array))
        return -SOTC_ERROR;

    json_object_array_add(j_region_state_array, j_state);

    return SOTC_OK;
}

/* Translate json representation to the internal structure */
int sotc_state_deserialize(struct sotc_model *model,
                           json_object *j_state,
                           struct sotc_region *region,
                           struct sotc_state **out)
{
    int rc = SOTC_OK;
    struct sotc_state *state;
    struct sotc_entry_exit *s_exit;
    struct sotc_entry_exit *s_entry;
    json_object *j_state_name;
    json_object *j_entries = NULL;
    json_object *k_entry_name = NULL;
    json_object *j_exits = NULL;
    json_object *j_entry = NULL;
    json_object *j_exit = NULL;
    json_object *jobj;

    state = malloc(sizeof(struct sotc_state));
    memset(state, 0, sizeof(*state));

    (*out) = state;

    if (!json_object_object_get_ex(j_state, "name", &j_state_name))
    {
        L_ERR("Missing name property, aborting");
        rc = -SOTC_ERR_PARSE;
        goto err_out;
    }

    if (!json_object_object_get_ex(j_state, "x", &jobj))
        state->x = 0;
    else
        state->x = json_object_get_double(jobj);

    if (!json_object_object_get_ex(j_state, "y", &jobj))
        state->y = 0;
    else
        state->y = json_object_get_double(jobj);

    if (!json_object_object_get_ex(j_state, "width", &jobj))
        state->w = 0;
    else
        state->w = json_object_get_double(jobj);

    if (!json_object_object_get_ex(j_state, "height", &jobj))
        state->h = 0;
    else
        state->h = json_object_get_double(jobj);

    state->name = strdup(json_object_get_string(j_state_name));
    state->parent_region = region;

    if (json_object_object_get_ex(j_state, "entries", &j_entries)) {
        size_t n_entries = json_object_array_length(j_entries);

        for (int n = 0; n < n_entries; n++)
        {
            j_entry = json_object_array_get_idx(j_entries, n);
            json_object *j_entry_id;
            uuid_t entry_id;
            if (json_object_object_get_ex(j_entry, "id", &j_entry_id)) {
                uuid_parse(json_object_get_string(j_entry_id), entry_id);
                rc = sotc_state_add_entry(model, state, entry_id);
                if (rc != SOTC_OK)
                    goto err_out;
            }
        }
    }

    if (json_object_object_get_ex(j_state, "exits", &j_exits)) {
        size_t n_entries = json_object_array_length(j_exits);

        for (int n = 0; n < n_entries; n++)
        {
            j_entry = json_object_array_get_idx(j_exits, n);
            json_object *j_exit_id;
            uuid_t exit_id;
            if (json_object_object_get_ex(j_exit, "id", &j_exit_id)) {
                uuid_parse(json_object_get_string(j_exit_id), exit_id);
                rc = sotc_state_add_exit(model, state, exit_id);
                if (rc != SOTC_OK)
                    goto err_out;
            }
        }
    }

    L_DEBUG("Loading state %s", state->name);

    return rc;
err_out:
    free(state);
    return rc;
}

int sotc_state_add_exit(struct sotc_model *model,
                        struct sotc_state *state,
                        uuid_t id)
{
    struct sotc_action *action;
    int rc;

    rc = sotc_model_get_action(model, id, SOTC_ACTION_EXIT, &action);

    if (rc != SOTC_OK)
        return rc;

    struct sotc_action_ref *list = state->exits;

    if (list == NULL) {
        list = malloc(sizeof(struct sotc_action_ref));
        memset(list, 0, sizeof(*list));
        list->act = action;
    } else {
        while (list->next)
            list = list->next;
        list->next = malloc(sizeof(struct sotc_action_ref));
        memset(list->next, 0, sizeof(*list->next));
        list->next->act = action;
    }

    return SOTC_OK;
}

int sotc_state_add_entry(struct sotc_model *model,
                         struct sotc_state *state,
                         uuid_t id)
{
    struct sotc_action *action;
    int rc;

    rc = sotc_model_get_action(model, id, SOTC_ACTION_ENTRY, &action);

    if (rc != SOTC_OK) {
        char uuid_str[37];
        uuid_unparse(id, uuid_str);
        L_ERR("Unkown entry action function %s", uuid_str);
        return rc;
    }

    L_DEBUG("Adding entry action '%s' to state '%s'", action->name, state->name);

    struct sotc_action_ref *list = state->entries;

    if (list == NULL) {
        list = malloc(sizeof(struct sotc_action_ref));
        memset(list, 0, sizeof(*list));
        list->act = action;
        state->entries = list;
    } else {
        while (list->next)
            list = list->next;
        list->next = malloc(sizeof(struct sotc_action_ref));
        memset(list->next, 0, sizeof(*list->next));
        list->next->act = action;
    }

    return SOTC_OK;
}

int sotc_state_get_entries(struct sotc_state *state,
                           struct sotc_action_ref **list)
{
    (*list) = state->entries;
    return SOTC_OK;
}

int sotc_state_get_exits(struct sotc_state *state,
                         struct sotc_action_ref **list)
{
    (*list) = state->exits;
    return SOTC_OK;
}

static int delete_action_ref(struct sotc_action_ref **list, uuid_t id)
{
    struct sotc_action_ref *tmp, *prev;

    tmp = *list;
    prev = NULL;

    while (tmp) {
        if (uuid_compare(tmp->act->id, id) == 0) {
            if (prev == NULL) {
                *list = tmp->next;
                free(tmp);
                return SOTC_OK;
            } else {
                prev->next = tmp->next;
                free(tmp);
                return SOTC_OK;
            }
        }

        prev = tmp;
        tmp = tmp->next;
    }
}

int sotc_state_delete_entry(struct sotc_state *state,
                            uuid_t id)
{
    return delete_action_ref(&state->entries, id);
}

int sotc_state_delete_exit(struct sotc_state *state,
                            uuid_t id)
{
    return delete_action_ref(&state->exits, id);
}
