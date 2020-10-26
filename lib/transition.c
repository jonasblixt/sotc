#include <stdio.h>
#include <string.h>
#include <sotc/sotc.h>
#include <sotc/model.h>
#include <json.h>

int sotc_transition_set_trigger(struct sotc_transition *transition, uuid_t id)
{
    return SOTC_OK;
}

int sotc_transition_add_guard(struct sotc_transition *transition, uuid_t id)
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

int sotc_transition_add_action(struct sotc_transition *transition, uuid_t id)
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

int sotc_transition_add_state_condition(struct sotc_transition *transition,
                                        uuid_t id, bool positive)
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
