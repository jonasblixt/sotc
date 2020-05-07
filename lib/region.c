#include <stdio.h>
#include <string.h>
#include <tcm/tcm.h>
#include <tcm/model.h>
#include <json.h>


int tcm_add_region(struct tcm_state *state, bool off_page,
                     struct tcm_region **out)
{
    struct tcm_region *region = malloc(sizeof(struct tcm_region));

    if (!region)
        return -TCM_ERR_MEM;

    memset(region, 0, sizeof(*region));

    (*out) = region;

    if (state->last_region)
        state->last_region->next = region;
    if (!state->regions)
        state->regions = region;

    state->last_region = region;

    return TCM_OK;
}

int tcm_set_region_name(struct tcm_region *region, const char *name)
{
    region->name = strdup(name);
    return TCM_OK;
}

int tcm_region_append_state(struct tcm_region *r, struct tcm_state *state)
{
    if (!r->last_state)
    {
        r->last_state = state;
    }

    if (!r->state)
    {
        r->state = state;
    }
    else
    {
        r->last_state->next = state;
        r->last_state = state;
    }

    return TCM_OK;
}

/* Translate the internal structure to json */
int tcm_region_serialize(struct tcm_region *region, json_object *state,
                         json_object **out)
{
    json_object *j_region = json_object_new_object();
    json_object *j_name = json_object_new_string(region->name);
    json_object *j_offpage = json_object_new_boolean(false);
    json_object *j_states = json_object_new_array();

    json_object_object_add(j_region, "name", j_name);
    json_object_object_add(j_region, "off_page", j_offpage);
    json_object_object_add(j_region, "states", j_states);

    if (state)
    {

        json_object *j_state_region_array;

        if (!json_object_object_get_ex(state, "region", &j_state_region_array))
            return -TCM_ERROR;

        json_object_array_add(j_state_region_array, j_region);
    }

    (*out) = j_region;

    return TCM_OK;
}

/* Translate json representation to the internal structure */
int tcm_region_deserialize(json_object *j_r, struct tcm_state *state,
                            struct tcm_region **out)
{
    json_object *jobj;
    struct tcm_region *r;

    r = malloc(sizeof(struct tcm_region));
    memset(r, 0, sizeof(*r));

    (*out) = r;


    if (!json_object_object_get_ex(j_r, "name", &jobj))
    {
        L_INFO("Could not read name property");
        r->name = strdup("No name");
    }
    else
    {
        r->name = strdup(json_object_get_string(jobj));
    }

    r->parent_state = state;

    return TCM_OK;
}
