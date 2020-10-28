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

    L_DEBUG("Parsing transitions in state '%s'", state->name);

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

        if (!json_object_object_get_ex(j_t, "text-block", &j_text_block)) {
            L_ERR("Could not read ID");
            rc = -SOTC_ERR_PARSE;
            goto err_out;
        }

        rc = sotc_model_deserialize_coords(j_text_block,
                                     &transition->text_block_coords);

        /* Parse actions */
        if (json_object_object_get_ex(j_t, "actions", &j_actions)) {
            size_t n_entries = json_object_array_length(j_actions);
            L_DEBUG("Transition has %d actions", n_entries);

            for (int n = 0; n < n_entries; n++)
            {
                json_object *j_action;
                json_object *j_action_id;
                uuid_t action_uu;

                j_action = json_object_array_get_idx(j_actions, n);

                if (json_object_object_get_ex(j_action, "id", &j_action_id)) {
                    uuid_parse(json_object_get_string(j_action_id), action_uu);
                    rc = sotc_transition_add_action(model, transition,
                                                    action_uu);
                    if (rc != SOTC_OK)
                        goto err_out;
                }
            }
        }

        /* Parse guards */
        if (json_object_object_get_ex(j_t, "guards", &j_guards)) {
            size_t n_entries = json_object_array_length(j_guards);
            L_DEBUG("Transition has %d guards", n_entries);

            for (int n = 0; n < n_entries; n++)
            {
                json_object *j_guard;
                json_object *j_guard_id;
                uuid_t guard_uu;

                j_guard = json_object_array_get_idx(j_guards, n);

                if (json_object_object_get_ex(j_guard, "id", &j_guard_id)) {
                    uuid_parse(json_object_get_string(j_guard_id), guard_uu);
                    rc = sotc_transition_add_guard(model, transition,
                                                    guard_uu);
                    if (rc != SOTC_OK)
                        goto err_out;
                }
            }
        }

    }

    L_DEBUG("Loaded transition '%s' -> '%s'", transition->source.state->name,
                                              transition->dest.state->name);
    printf("  %s [", transition->trigger->name);

    for (struct sotc_action_ref *ref = transition->guard; ref; ref = ref->next) {
        printf("%s", ref->act->name);
        if (ref->next)
            printf(", ");
    }
    printf("] / ");

    for (struct sotc_action_ref *ref = transition->action; ref; ref = ref->next) {
        printf("%s", ref->act->name);
        if (ref->next)
            printf(", ");
    }
    printf("\n");


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
        free_action_ref_list(transition->action);
        free_action_ref_list(transition->guard);
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
    struct sotc_action *action;
    int rc;

    L_DEBUG("%s", __func__);

    rc = sotc_model_get_action(model, id, SOTC_ACTION_GUARD, &action);

    if (rc != SOTC_OK) {
        char uuid_str[37];
        uuid_unparse(id, uuid_str);
        L_ERR("Unkown guard function %s", uuid_str);
        return rc;
    }

    L_DEBUG("Adding guard '%s' to transition", action->name);
    struct sotc_action_ref *list = transition->guard;

    if (list == NULL) {
        list = malloc(sizeof(struct sotc_action_ref));
        memset(list, 0, sizeof(*list));
        list->act = action;
        transition->guard = list;
    } else {
        while (list->next)
            list = list->next;
        list->next = malloc(sizeof(struct sotc_action_ref));
        memset(list->next, 0, sizeof(*list->next));
        list->next->act = action;
    }

    return SOTC_OK;
}

int sotc_transition_delete_guard(struct sotc_transition *transition, uuid_t id)
{
    return SOTC_OK;
}

struct sotc_action_ref *sotc_transition_get_guards(struct sotc_transition *t)
{
    return t->guard;
}

int sotc_transition_add_action(struct sotc_model *model,
                               struct sotc_transition *transition,
                               uuid_t id)
{
    struct sotc_action *action;
    int rc;

    rc = sotc_model_get_action(model, id, SOTC_ACTION_ACTION, &action);

    if (rc != SOTC_OK) {
        char uuid_str[37];
        uuid_unparse(id, uuid_str);
        L_ERR("Unkown action function %s", uuid_str);
        return rc;
    }

    L_DEBUG("Adding action '%s' to transition", action->name);
    struct sotc_action_ref *list = transition->action;

    if (list == NULL) {
        list = malloc(sizeof(struct sotc_action_ref));
        memset(list, 0, sizeof(*list));
        list->act = action;
        transition->action = list;
    } else {
        while (list->next)
            list = list->next;
        list->next = malloc(sizeof(struct sotc_action_ref));
        memset(list->next, 0, sizeof(*list->next));
        list->next->act = action;
    }

    return SOTC_OK;
}

int sotc_transition_delete_action(struct sotc_transition *transition, uuid_t id)
{
    return SOTC_OK;
}

struct sotc_action_ref *sotc_transition_get_actions(struct sotc_transition *t)
{
    return t->action;
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

struct sotc_transition_state_condition *
sotc_transition_get_state_conditions(struct sotc_transition *t)
{
    return t->state_conditions;
}
