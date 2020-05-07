#include <stdio.h>
#include <string.h>
#include <tcm/tcm.h>
#include <tcm/model.h>
#include <json.h>


int tcm_add_state(struct tcm_region *region, const char *name,
                    struct tcm_state **out)
{
    struct tcm_state *state = malloc(sizeof(struct tcm_state));

    if (!state)
        return -TCM_ERR_MEM;

    memset(state, 0, sizeof(*state));

    (*out) = state;

    state->name = strdup(name);

    if (region->last_state)
        region->last_state->next = state;
    if (!region->state)
        region->state = state;

    region->last_state = state;

    return TCM_OK;
}

int tcm_state_append_region(struct tcm_state *state, struct tcm_region *r)
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

    return TCM_OK;
}

/* Translate the internal structure to json */
int tcm_state_serialize(struct tcm_state *state, json_object *region,
                        json_object **out)
{
    json_object *j_state = json_object_new_object();
    json_object *j_name = json_object_new_string(state->name);
    json_object *j_kind = json_object_new_string("state");
    json_object *j_region = json_object_new_array();

    json_object_object_add(j_state, "name", j_name);
    json_object_object_add(j_state, "kind", j_kind);
    json_object_object_add(j_state, "region", j_region);

    (*out) = j_state;

    json_object *j_region_state_array;

    if (!json_object_object_get_ex(region, "states", &j_region_state_array))
        return -TCM_ERROR;

    json_object_array_add(j_region_state_array, j_state);

    return TCM_OK;
}

/* Translate json representation to the internal structure */
int tcm_state_deserialize(json_object *j_state, struct tcm_region *region,
                          struct tcm_state **out)
{
    int rc = TCM_OK;
    struct tcm_state *state;
    json_object *j_state_name;

    state = malloc(sizeof(struct tcm_state));
    memset(state, 0, sizeof(*state));

    (*out) = state;

    if (!json_object_object_get_ex(j_state, "name", &j_state_name))
    {
        L_ERR("Missing name property, aborting");
        rc = -TCM_ERR_PARSE;
        goto err_out;
    }

    state->name = strdup(json_object_get_string(j_state_name));
    state->parent_region = region;

    L_DEBUG("Loading state %s", state->name);

    return rc;
err_out:
    free(state);
    return rc;
}
