#include <stdio.h>
#include <string.h>
#include <sotc/sotc.h>
#include <sotc/model.h>
#include <json.h>

static int parse_state_ref(struct sotc_model *model,
                           json_object *j_ref,
                           struct sotc_transition_state_ref *result)
{
    int rc;
    json_object *j_state;
    json_object *j_side;
    json_object *j_offset;
    struct sotc_state *state = NULL;
    double offset;
    enum sotc_side side;
    uuid_t state_uu;

    if (!json_object_object_get_ex(j_ref, "state", &j_state)) {
        L_ERR("Could not parse state");
        return -SOTC_ERROR;
    }

    uuid_parse(json_object_get_string(j_state), state_uu);

    if (!json_object_object_get_ex(j_ref, "side", &j_side)) {
        L_ERR("Could not parse side");
        return -SOTC_ERROR;
    }

    if (!json_object_object_get_ex(j_ref, "offset", &j_offset)) {
        L_ERR("Could not parse offset");
        return -SOTC_ERROR;
    }

    L_DEBUG("Searching for state '%s'", json_object_get_string(j_state));
    state = sotc_model_get_state_from_uuid(model, state_uu);

    if (state == NULL) {
        L_ERR("Could not find state");
        return -SOTC_ERROR;
    }
    if (strcmp(json_object_get_string(j_side), "left") == 0) {
        result->side = SOTC_SIDE_LEFT;
    } else if (strcmp(json_object_get_string(j_side), "right") == 0) {
        result->side = SOTC_SIDE_RIGHT;
    } else if (strcmp(json_object_get_string(j_side), "top") == 0) {
        result->side = SOTC_SIDE_TOP;
    } else if (strcmp(json_object_get_string(j_side), "bottom") == 0) {
        result->side = SOTC_SIDE_BOTTOM;
    } else {
        L_ERR("Uknown side: '%s'", json_object_get_string(j_side));
        return -SOTC_ERROR;
    }

    result->offset = json_object_get_double(j_offset);
    result->state = state;

    return SOTC_OK;
}

int sotc_transition_deserialize(struct sotc_model *model,
                                struct sotc_state *state,
                                json_object *j_transitions_list)
{
    int rc = SOTC_OK;
    json_object *j_t;
    json_object *j_id;
    json_object *j_trigger;
    json_object *j_source;
    json_object *j_dest;
    json_object *j_text_block;
    json_object *j_vertices;
    json_object *j_guards;
    json_object *j_actions;
    json_object *j_state_conditions;
    struct sotc_transition *transition;
    uuid_t trigger_uu;

    transition = malloc(sizeof(struct sotc_transition));

    if (transition == NULL)
        return -SOTC_ERROR;

    memset(transition, 0, sizeof(*transition));

    size_t n_entries = json_object_array_length(j_transitions_list);

    for (int n = 0; n < n_entries; n++) {
        j_t = json_object_array_get_idx(j_transitions_list, n);

        if (!json_object_object_get_ex(j_t, "id", &j_id)) {
            L_ERR("Could not read ID");
            rc = -SOTC_ERR_PARSE;
            goto err_out;
        }

        uuid_parse(json_object_get_string(j_id), transition->id);

        if (!json_object_object_get_ex(j_t, "trigger", &j_trigger)) {
            L_ERR("Could not trigger ID");
            rc = -SOTC_ERR_PARSE;
            goto err_out;
        }

        uuid_parse(json_object_get_string(j_trigger), trigger_uu);
        transition->trigger = sotc_model_get_trigger_from_uuid(model,
                                                               trigger_uu);

        if (transition->trigger == NULL) {
            L_ERR("Could not find trigger");
            rc = -SOTC_ERROR;
            goto err_out;
        }

        if (!json_object_object_get_ex(j_t, "source", &j_source)) {
            L_ERR("Could find source state");
            rc = -SOTC_ERR_PARSE;
            goto err_out;
        }

        rc = parse_state_ref(model, j_source, &transition->source);

        if (rc != SOTC_OK) {
            L_ERR("Could not parse source state");
            goto err_out;
        }

        if (!json_object_object_get_ex(j_t, "dest", &j_dest)) {
            L_ERR("Could find dest state");
            rc = -SOTC_ERR_PARSE;
            goto err_out;
        }

        rc = parse_state_ref(model, j_dest, &transition->dest);

        if (rc != SOTC_OK) {
            L_ERR("Could not parse source state");
            goto err_out;
        }
    }

    L_DEBUG("Loaded transition '%s' -> '%s' / '%s'", transition->source.state->name,
                                            transition->dest.state->name,
                                            transition->trigger->name);

    if (state->transition == NULL) {
        state->transition = transition;
    } else {
        struct sotc_transition *list = state->transition;

        while (list->next)
            list = list->next;
        list->next = transition;
    }

    return rc;
err_out:
    free(transition);
    return rc;
}

int sotc_transition_free(struct sotc_transition *transition)
{
    struct sotc_transition *list = transition;
    struct sotc_transition *tmp;

    if (transition == NULL)
        return SOTC_OK;

    while (list)
    {
        tmp = list->next;
        free(list);
        list = tmp;
    }

    return SOTC_OK;
}

int sotc_transition_set_trigger(struct sotc_model *model,
                                struct sotc_transition *transition,
                                uuid_t id)
{
    return SOTC_OK;
}

int sotc_transition_add_guard(struct sotc_model *model,
                              struct sotc_transition *transition,
                              uuid_t id)
{
    return SOTC_OK;
}

int sotc_transition_delete_guard(struct sotc_transition *transition, uuid_t id)
{
    return SOTC_OK;
}

struct sotc_action_ref *sotc_transition_get_guards(struct sotc_transition *t)
{
    return NULL;
}

int sotc_transition_add_action(struct sotc_model *model,
                               struct sotc_transition *transition,
                               uuid_t id)
{
    return SOTC_OK;
}

int sotc_transition_delete_action(struct sotc_transition *transition, uuid_t id)
{
    return SOTC_OK;
}

struct sotc_action_ref *sotc_transition_get_actions(struct sotc_transition *t)
{
    return NULL;
}

int sotc_transition_add_state_condition(struct sotc_model *model,
                                        struct sotc_transition *transition,
                                        uuid_t id,
                                        bool positive)
{
    return SOTC_OK;
}

int sotc_transition_delete_state_condition(struct sotc_transition *transition,
                                            uuid_t id)
{
    return SOTC_OK;
}

struct sotc_state_ref *sotc_transition_get_state_conditions(struct sotc_transition *t)
{
    return NULL;
}
