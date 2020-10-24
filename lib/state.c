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

int sotc_state_set_size(struct sotc_state *s, int x, int y)
{
    s->w = x;
    s->h = y;
    return SOTC_OK;
}

int sotc_state_set_xy(struct sotc_state *s, int x, int y)
{
    s->x = x;
    s->y = y;
    return SOTC_OK;
}

int sotc_state_get_size(struct sotc_state *s, int *x, int *y)
{
    (*x) = s->w;
    (*y) = s->h;
    return SOTC_OK;
}

int sotc_state_get_xy(struct sotc_state *s, int *x, int *y)
{
    (*x) = s->x;
    (*y) = s->y;
    return SOTC_OK;
}

/* Translate the internal structure to json */
int sotc_state_serialize(struct sotc_state *state, json_object *region,
                        json_object **out)
{
    json_object *j_state = json_object_new_object();
    json_object *j_name = json_object_new_string(state->name);
    json_object *j_kind = json_object_new_string("state");
    json_object *j_region = json_object_new_array();

    json_object_object_add(j_state, "name", j_name);
    json_object_object_add(j_state, "kind", j_kind);

    json_object_object_add(j_state, "width",
                json_object_new_int(state->w));

    json_object_object_add(j_state, "height",
                json_object_new_int(state->h));

    json_object_object_add(j_state, "x",
                json_object_new_int(state->x));
    json_object_object_add(j_state, "y",
                json_object_new_int(state->y));

    json_object_object_add(j_state, "region", j_region);

    (*out) = j_state;

    json_object *j_region_state_array;

    if (!json_object_object_get_ex(region, "states", &j_region_state_array))
        return -SOTC_ERROR;

    json_object_array_add(j_region_state_array, j_state);

    return SOTC_OK;
}

/* Translate json representation to the internal structure */
int sotc_state_deserialize(json_object *j_state, struct sotc_region *region,
                          struct sotc_state **out)
{
    int rc = SOTC_OK;
    struct sotc_state *state;
    struct sotc_entry_exit *s_exit;
    struct sotc_entry_exit *s_entry;
    json_object *j_state_name;
    json_object *j_entries = NULL;
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
        state->x = json_object_get_int(jobj);

    if (!json_object_object_get_ex(j_state, "y", &jobj))
        state->y = 0;
    else
        state->y = json_object_get_int(jobj);

    if (!json_object_object_get_ex(j_state, "width", &jobj))
        state->w = 0;
    else
        state->w = json_object_get_int(jobj);

    if (!json_object_object_get_ex(j_state, "height", &jobj))
        state->h = 0;
    else
        state->h = json_object_get_int(jobj);

    if (json_object_object_get_ex(j_state, "entry", &j_entries)) {
        size_t n_entries = json_object_array_length(j_entries);

        for (int n = 0; n < n_entries; n++)
        {
            j_entry = json_object_array_get_idx(j_entries, n)
            s_entry = malloc(sizeof(s_entry));
            memset(s_entry, 0, sizeof(*s_entry));
            s_entry->name = strdup();
        }
    }

    state->name = strdup(json_object_get_string(j_state_name));
    state->parent_region = region;

    L_DEBUG("Loading state %s", state->name);

    return rc;
err_out:
    free(state);
    return rc;
}
