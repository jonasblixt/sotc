#ifndef INCLUDE_TCM_MODEL_H_
#define INCLUDE_TCM_MODEL_H_

#include <stdint.h>
#include <stdbool.h>
#include <json.h>

enum tcm_transition_kind
{
    TCM_TRANSITION_EXTERNAL,
    TCM_TRANSITION_INTERNAL,
    TCM_TRANSITION_LOCAL,
};

enum tcm_state_kind
{
    TCM_STATE_SIMPLE,
    TCM_STATE_INIT,
    TCM_STATE_FINAL,
    TCM_STATE_SHALLOW_HISTORY,
    TCM_STATE_DEEP_HISTORY,
    TCM_STATE_EXIT_POINT,
    TCM_STATE_ENTRY_POINT,
    TCM_STATE_JOIN,
    TCM_STATE_FORK,
    TCM_STATE_CHOICE,
    TCM_STATE_JUNCTION,
    TCM_STATE_TERMINATE,
};

struct tcm_action
{
    const char *id;
    const char *name;
    struct tcm_action *next;
};

struct tcm_guard
{
    const char *id;
    const char *name;
    struct tcm_guard *next;
};

struct tcm_entry_exit
{
    const char *id;
    const char *name;
    struct tcm_entry_exit *next;
};

struct tcm_doact
{
    const char *id;
    const char *name;
    struct tcm_doact *next;
};

struct tcm_transition
{
    const char *id;
    const char *name;
    const char *trigger_name;
    int trigger;
    bool defer;
    enum tcm_transition_kind kind;
    struct tcm_action *action;
    struct tcm_guard *guard;
    struct tcm_state *source;
    struct tcm_state *dest;
    struct tcm_transition *next;
};

struct tcm_region
{
    const char *id;
    const char *name;
    bool has_history;
    struct tcm_state *current;
    struct tcm_state *history;
    struct tcm_state *state;
    struct tcm_transition *transition;
    struct tcm_state *parent_state;
    struct tcm_region *next;
};

struct tcm_state
{
    const char *id;
    const char *name;
    enum tcm_state_kind kind;
    struct tcm_entry_exit *entry;
    struct tcm_doact *doact;
    struct tcm_entry_exit *exit;
    struct tcm_region *region;
    struct tcm_region *parent_region;
    struct tcm_machine *submachine;
    struct tcm_state *next;
};

struct tcm_model
{
    json_object *jroot;
    struct tcm_state *root;
};

int tcm_model_load(const char *filename, struct tcm_model **model);
int tcm_model_write(const char *filename, struct tcm_model *model);
int tcm_model_free(struct tcm_model *model);

#endif  // INCLUDE_TCM_MODEL_H_
