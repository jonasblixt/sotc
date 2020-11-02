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

    uuid_generate_random(state->id);

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
    char s_uuid_str[37];
    json_object *j_state = json_object_new_object();
    json_object *j_name = json_object_new_string(state->name);
    json_object *j_kind;
    json_object *j_region = json_object_new_array();
    json_object *j_transitions = json_object_new_array();

    uuid_unparse(state->id, s_uuid_str);
    json_object *j_id = json_object_new_string(s_uuid_str);

    json_object *j_entries = json_object_new_array();
    json_object *j_exits = json_object_new_array();

    json_object_object_add(j_state, "id", j_id);
    json_object_object_add(j_state, "name", j_name);

    switch (state->kind) {
        case SOTC_STATE_NORMAL:
            j_kind = json_object_new_string("state");
        break;
        case SOTC_STATE_INIT:
            j_kind = json_object_new_string("init");
        break;
        case SOTC_STATE_FINAL:
            j_kind = json_object_new_string("final");
        break;
        case SOTC_STATE_SHALLOW_HISTORY:
            j_kind = json_object_new_string("shallow-history");
        break;
        case SOTC_STATE_DEEP_HISTORY:
            j_kind = json_object_new_string("deep-history");
        break;
        case SOTC_STATE_EXIT_POINT:
            j_kind = json_object_new_string("exit-point");
        break;
        case SOTC_STATE_ENTRY_POINT:
            j_kind = json_object_new_string("entry-point");
        break;
        case SOTC_STATE_JOIN:
            j_kind = json_object_new_string("join");
        break;
        case SOTC_STATE_FORK:
            j_kind = json_object_new_string("fork");
        break;
        case SOTC_STATE_CHOICE:
            j_kind = json_object_new_string("choice");
        break;
        case SOTC_STATE_JUNCTION:
            j_kind = json_object_new_string("junction");
        break;
        case SOTC_STATE_TERMINATE:
            j_kind = json_object_new_string("terminate");
        break;
        default:
            L_ERR("Unknown state type %i", state->kind);
            rc = -1;
            goto err_out;
    }


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
        goto err_out;

    rc = serialize_action_list(state->exits, j_exits);
    if (rc != SOTC_OK)
        goto err_out;

    json_object_object_add(j_state, "entries", j_entries);
    json_object_object_add(j_state, "exits", j_exits);
    json_object_object_add(j_state, "region", j_region);

    /* Serialize transitions owned by this state */
    rc = sotc_transitions_serialize(state, j_transitions);

    if (rc != SOTC_OK) {
        L_ERR("Could not serialize transitions");
        json_object_put(j_transitions);
        goto err_out;
    }

    json_object_object_add(j_state, "transitions", j_transitions);

    (*out) = j_state;

    json_object *j_region_state_array;

    if (!json_object_object_get_ex(region, "states", &j_region_state_array))
        return -SOTC_ERROR;

    json_object_array_add(j_region_state_array, j_state);

    return SOTC_OK;
err_out:
    json_object_put(j_state);
    return rc;
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
    json_object *j_id = NULL;
    json_object *j_state_kind = NULL;
    json_object *jobj;

    state = malloc(sizeof(struct sotc_state));
    memset(state, 0, sizeof(*state));

    (*out) = state;

    if (!json_object_object_get_ex(j_state, "id", &j_id)) {
        L_ERR("Could not read ID");
        rc = -SOTC_ERR_PARSE;
        goto err_out;
    }

    uuid_parse(json_object_get_string(j_id), state->id);

    if (!json_object_object_get_ex(j_state, "name", &j_state_name))
    {
        L_ERR("Missing name property, aborting");
        rc = -SOTC_ERR_PARSE;
        goto err_out;
    }

    if (!json_object_object_get_ex(j_state, "kind", &j_state_kind))
    {
        L_ERR("Missing kind property, aborting");
        rc = -SOTC_ERR_PARSE;
        goto err_out;
    }

    const char *state_kind = json_object_get_string(j_state_kind);

    if (strcmp(state_kind, "state") == 0) {
        state->kind = SOTC_STATE_NORMAL;
        state->resizeable = true;
    } else if (strcmp(state_kind, "init") == 0) {
        state->kind = SOTC_STATE_INIT;
    } else if (strcmp(state_kind, "final") == 0) {
        state->kind = SOTC_STATE_FINAL;
    } else if (strcmp(state_kind, "shallow-history") == 0) {
        state->kind = SOTC_STATE_SHALLOW_HISTORY;
    } else if (strcmp(state_kind, "deep-history") == 0) {
        state->kind = SOTC_STATE_DEEP_HISTORY;
    } else if (strcmp(state_kind, "exit-point") == 0) {
        state->kind = SOTC_STATE_EXIT_POINT;
    } else if (strcmp(state_kind, "entry-point") == 0) {
        state->kind = SOTC_STATE_ENTRY_POINT;
    } else if (strcmp(state_kind, "join") == 0) {
        state->kind = SOTC_STATE_JOIN;
        state->resizeable = true;
    } else if (strcmp(state_kind, "fork") == 0) {
        state->kind = SOTC_STATE_FORK;
        state->resizeable = true;
    } else if (strcmp(state_kind, "choice") == 0) {
        state->kind = SOTC_STATE_CHOICE;
        state->resizeable = true;
    } else if (strcmp(state_kind, "junction") == 0) {
        state->kind = SOTC_STATE_JUNCTION;
    } else if (strcmp(state_kind, "terminate") == 0) {
        state->kind = SOTC_STATE_TERMINATE;
    } else {
        L_ERR("Unknown state kind '%s'", state_kind);
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
        L_DEBUG("State '%s' has %d entry actions", state->name, n_entries);

        for (int n = 0; n < n_entries; n++)
        {
            j_entry = json_object_array_get_idx(j_entries, n);
            json_object *j_entry_id;
            uuid_t entry_id;
            if (json_object_object_get_ex(j_entry, "id", &j_entry_id)) {
                uuid_parse(json_object_get_string(j_entry_id), entry_id);
                rc = sotc_state_add_entry(model, state, entry_id);
                if (rc != SOTC_OK)
                    goto err_free_name_out;
            }
        }
    }

    if (json_object_object_get_ex(j_state, "exits", &j_exits)) {
        size_t n_entries = json_object_array_length(j_exits);

        L_DEBUG("State '%s' has %d exit actions", state->name, n_entries);
        for (int n = 0; n < n_entries; n++)
        {
            j_exit = json_object_array_get_idx(j_exits, n);
            json_object *j_exit_id;
            uuid_t exit_id;
            if (json_object_object_get_ex(j_exit, "id", &j_exit_id)) {
                uuid_parse(json_object_get_string(j_exit_id), exit_id);
                rc = sotc_state_add_exit(model, state, exit_id);
                if (rc != SOTC_OK)
                    goto err_free_name_out;
            }
        }
    }

    /* NOTE: Transitions are loaded in pass 2 since the whole object tree
     *  must be available for state resolution in transitions */

    L_DEBUG("Loading state %s", state->name);

    return rc;
err_free_name_out:
    free((void *) state->name);
err_out:
    free_action_ref_list(state->entries);
    free_action_ref_list(state->exits);
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

    if (rc != SOTC_OK) {
        char uuid_str[37];
        uuid_unparse(id, uuid_str);
        L_ERR("Unkown exit action function %s", uuid_str);
        return rc;
    }

    L_DEBUG("Adding exit action '%s' to state '%s'", action->name, state->name);
    struct sotc_action_ref *list = state->exits;

    if (list == NULL) {
        list = malloc(sizeof(struct sotc_action_ref));
        memset(list, 0, sizeof(*list));
        list->act = action;
        state->exits = list;
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

int sotc_state_add_transition(struct sotc_state *source,
                              struct sotc_state *dest,
                              struct sotc_transition **transition)
{
    struct sotc_transition *t, *t_tmp;

    t = malloc(sizeof(*t));

    if (t == NULL)
        return -SOTC_ERROR;

    memset(t, 0, sizeof(*t));

    if (transition) {
        (*transition) = t;
    }

    uuid_generate_random(t->id);
    t->source.state = source;
    t->dest.state = dest;

    if (source->transition == NULL) {
        source->transition = t;
    } else {
        t_tmp = source->transition;

        while (t_tmp->next)
            t_tmp = t_tmp->next;

        t_tmp->next = t;
    }

    return SOTC_OK;
}

int sotc_state_delete_transition(struct sotc_transition *transition)
{
    struct sotc_state *s = transition->source.state;


    return SOTC_OK;
}

int sotc_state_get_transitions(struct sotc_state *state,
                               struct sotc_transition **transitions)
{
    (*transitions) = state->transition;
    return SOTC_OK;
}
