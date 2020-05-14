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
    enum tcm_transition_kind kind;
    struct tcm_action *action;
    struct tcm_guard *guard;
    struct tcm_state *source;
    struct tcm_state *dest;
    struct tcm_transition *next;
};

struct tcm_region
{
    const char *name;
    bool off_page;
    int x,y;
    int w;
    int h;
    struct tcm_state *state;
    struct tcm_transition *transition;
    struct tcm_state *parent_state;
    struct tcm_state *last_state;
    struct tcm_region *next;
};

struct tcm_state
{
    const char *name;
    int x;
    int y;
    int w;
    int h;
    enum tcm_state_kind kind;
    struct tcm_entry_exit *entries;
    struct tcm_doact *doacts;
    struct tcm_entry_exit *exits;
    struct tcm_region *regions;
    struct tcm_region *parent_region;
    struct tcm_region *last_region;
    struct tcm_state *next;
};

struct tcm_model
{
    json_object *jroot;
    struct tcm_region *root;
    const char *name;
    int version;
};

int tcm_model_load(const char *filename, struct tcm_model **model);
int tcm_model_create(struct tcm_model **model, const char *name);
int tcm_model_write(const char *filename, struct tcm_model *model);
int tcm_model_free(struct tcm_model *model);

/* Region api */
int tcm_add_region(struct tcm_state *state, bool off_page,
                     struct tcm_region **out);
int tcm_set_region_name(struct tcm_region *region, const char *name);

int tcm_region_append_state(struct tcm_region *r, struct tcm_state *state);

int tcm_region_serialize(struct tcm_region *region, json_object *state,
                         json_object **out);

int tcm_region_deserialize(json_object *j_r, struct tcm_state *state,
                            struct tcm_region **out);

int tcm_region_set_size(struct tcm_region *r, int x, int y);
int tcm_region_set_xy(struct tcm_region *r, int x, int y);

int tcm_region_get_size(struct tcm_region *r, int *x, int *y);
int tcm_region_get_xy(struct tcm_region *r, int *x, int *y);

/* State api */

int tcm_add_state(struct tcm_region *region, const char *name,
                    struct tcm_state **out);

int tcm_add_exit(struct tcm_entry_exit **exit_p,
                 struct tcm_state *state,
                 const char *action_name);

int tcm_add_entry(struct tcm_entry_exit **entry_p,
                  struct tcm_state *state,
                  const char *action_name);

int tcm_add_transition(struct tcm_transition **transition,
                       struct tcm_state *source,
                       struct tcm_state *dest);

int tcm_state_append_region(struct tcm_state *state, struct tcm_region *r);

int tcm_state_serialize(struct tcm_state *state, json_object *region,
                        json_object **out);

int tcm_state_deserialize(json_object *j_state, struct tcm_region *region,
                          struct tcm_state **out);


int tcm_state_set_size(struct tcm_state *s, int x, int y);
int tcm_state_set_xy(struct tcm_state *s, int x, int y);

int tcm_state_get_size(struct tcm_state *s, int *x, int *y);
int tcm_state_get_xy(struct tcm_state *s, int *x, int *y);

const char * tcm_model_name(struct tcm_model *model);

#endif  // INCLUDE_TCM_MODEL_H_
