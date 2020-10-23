#include <stdio.h>
#include <string.h>
#include <sotc/sotc.h>
#include <sotc/model.h>
#include <json.h>


int sotc_add_region(struct sotc_state *state, bool off_page,
                     struct sotc_region **out)
{
    struct sotc_region *region = malloc(sizeof(struct sotc_region));

    if (!region)
        return -SOTC_ERR_MEM;

    memset(region, 0, sizeof(*region));

    (*out) = region;

    if (state->last_region)
        state->last_region->next = region;
    if (!state->regions)
        state->regions = region;

    state->last_region = region;

    return SOTC_OK;
}

int sotc_set_region_name(struct sotc_region *region, const char *name)
{
    region->name = strdup(name);
    return SOTC_OK;
}

int sotc_region_append_state(struct sotc_region *r, struct sotc_state *state)
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

    return SOTC_OK;
}

int sotc_region_set_height(struct sotc_region *r, double h)
{
    r->h = h;
    return SOTC_OK;
}

int sotc_region_get_height(struct sotc_region *r, double *h)
{
    (*h) = r->h;
    return SOTC_OK;
}

/* Translate the internal structure to json */
int sotc_region_serialize(struct sotc_region *region, json_object *state,
                         json_object **out)
{
    json_object *j_region = json_object_new_object();
    json_object *j_name = json_object_new_string(region->name);
    json_object *j_offpage = json_object_new_boolean(false);
    json_object *j_states = json_object_new_array();

    json_object_object_add(j_region, "name", j_name);
    json_object_object_add(j_region, "off_page", j_offpage);
    json_object_object_add(j_region, "height",
                json_object_new_int(region->h));

    json_object_object_add(j_region, "states", j_states);

    if (state)
    {

        json_object *j_state_region_array;

        if (!json_object_object_get_ex(state, "region", &j_state_region_array))
            return -SOTC_ERROR;

        json_object_array_add(j_state_region_array, j_region);
    }

    (*out) = j_region;

    return SOTC_OK;
}

/* Translate json representation to the internal structure */
int sotc_region_deserialize(json_object *j_r, struct sotc_state *state,
                            struct sotc_region **out)
{
    json_object *jobj;
    struct sotc_region *r;

    r = malloc(sizeof(struct sotc_region));
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

    if (!json_object_object_get_ex(j_r, "height", &jobj))
        r->h = 0.0;
    else
        r->h = json_object_get_double(jobj);

    r->parent_state = state;

    return SOTC_OK;
}
