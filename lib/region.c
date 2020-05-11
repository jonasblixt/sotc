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

int tcm_region_set_size(struct tcm_region *r, int x, int y)
{
    r->width = x;
    r->height = y;
    return TCM_OK;
}

int tcm_region_set_xy(struct tcm_region *r, int x, int y)
{
    r->x = x;
    r->y = y;
    return TCM_OK;
}

int tcm_region_get_size(struct tcm_region *r, int *x, int *y)
{
    (*x) = r->width;
    (*y) = r->height;
    return TCM_OK;
}

int tcm_region_get_xy(struct tcm_region *r, int *x, int *y)
{
    (*x) = r->x;
    (*y) = r->y;
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
    json_object_object_add(j_region, "width",
                json_object_new_int(region->width));

    json_object_object_add(j_region, "height",
                json_object_new_int(region->height));

    json_object_object_add(j_region, "x",
                json_object_new_int(region->x));
    json_object_object_add(j_region, "y",
                json_object_new_int(region->y));

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

    if (!json_object_object_get_ex(j_r, "x", &jobj))
        r->x = 0;
    else
        r->x = json_object_get_int(jobj);

    if (!json_object_object_get_ex(j_r, "y", &jobj))
        r->y = 0;
    else
        r->y = json_object_get_int(jobj);

    if (!json_object_object_get_ex(j_r, "width", &jobj))
        r->width = 0;
    else
        r->width = json_object_get_int(jobj);

    if (!json_object_object_get_ex(j_r, "height", &jobj))
        r->height = 0;
    else
        r->height = json_object_get_int(jobj);

    r->parent_state = state;

    return TCM_OK;
}
