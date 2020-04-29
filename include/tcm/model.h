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
    bool off_page;
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
    struct tcm_entry_exit *entries;
    struct tcm_doact *doacts;
    struct tcm_entry_exit *exits;
    struct tcm_region *regions;
    struct tcm_region *parent_region;
    struct tcm_state *next;
};

struct tcm_model
{
    json_object *jroot;
    struct tcm_region *root;
    const char *name;
};

int tcm_model_load(const char *filename, struct tcm_model **model);
int tcm_model_create(struct tcm_model **model, const char *name);
int tcm_model_write(const char *filename, struct tcm_model *model);
int tcm_model_free(struct tcm_model *model);

/* Region api */
int tcm_add_region(struct tcm_state **state, bool off_page);

/* State api */
int tcm_add_state(struct tcm_region **region, const char *name);

int tcm_add_exit(struct tcm_entry_exit **exit_p,
                 struct tcm_state *state,
                 const char *action_name);

int tcm_add_entry(struct tcm_entry_exit **entry_p,
                  struct tcm_state *state,
                  const char *action_name);

int tcm_add_transition(struct tcm_transition **transition,
                       struct tcm_state *source,
                       struct tcm_state *dest);

const char * tcm_model_name(struct tcm_model *model);

#endif  // INCLUDE_TCM_MODEL_H_
