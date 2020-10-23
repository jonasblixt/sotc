#ifndef INCLUDE_SOTC_MODEL_H_
#define INCLUDE_SOTC_MODEL_H_

#include <stdint.h>
#include <stdbool.h>
#include <json.h>

enum sotc_transition_kind
{
    SOTC_TRANSITION_EXTERNAL,
    SOTC_TRANSITION_INTERNAL,
    SOTC_TRANSITION_LOCAL,
};

enum sotc_state_kind
{
    SOTC_STATE_SIMPLE,
    SOTC_STATE_INIT,
    SOTC_STATE_FINAL,
    SOTC_STATE_SHALLOW_HISTORY,
    SOTC_STATE_DEEP_HISTORY,
    SOTC_STATE_EXIT_POINT,
    SOTC_STATE_ENTRY_POINT,
    SOTC_STATE_JOIN,
    SOTC_STATE_FORK,
    SOTC_STATE_CHOICE,
    SOTC_STATE_JUNCTION,
    SOTC_STATE_TERMINATE,
};

struct sotc_action
{
    const char *id;
    const char *name;
    struct sotc_action *next;
};

struct sotc_guard
{
    const char *id;
    const char *name;
    struct sotc_guard *next;
};

struct sotc_entry_exit
{
    const char *id;
    const char *name;
    struct sotc_entry_exit *next;
};

struct sotc_doact
{
    const char *id;
    const char *name;
    struct sotc_doact *next;
};

struct sotc_transition
{
    const char *id;
    const char *name;
    const char *trigger_name;
    int trigger;
    enum sotc_transition_kind kind;
    struct sotc_action *action;
    struct sotc_guard *guard;
    struct sotc_state *source;
    struct sotc_state *dest;
    struct sotc_transition *next;
};

struct sotc_region
{
    const char *name;
    bool off_page;
    double h;
    bool focus;
    struct sotc_state *state;
    struct sotc_transition *transition;
    struct sotc_state *parent_state;
    struct sotc_state *last_state;
    struct sotc_region *next;
};

struct sotc_state
{
    const char *name;
    int x;
    int y;
    int w;
    int h;
    bool focus;
    enum sotc_state_kind kind;
    struct sotc_entry_exit *entries;
    struct sotc_doact *doacts;
    struct sotc_entry_exit *exits;
    struct sotc_region *regions;
    struct sotc_region *parent_region;
    struct sotc_region *last_region;
    struct sotc_state *next;
};

struct sotc_model
{
    json_object *jroot;
    struct sotc_region *root;
    const char *name;
    int version;
};

int sotc_model_load(const char *filename, struct sotc_model **model);
int sotc_model_create(struct sotc_model **model, const char *name);
int sotc_model_write(const char *filename, struct sotc_model *model);
int sotc_model_free(struct sotc_model *model);

/* Region api */
int sotc_add_region(struct sotc_state *state, bool off_page,
                     struct sotc_region **out);
int sotc_set_region_name(struct sotc_region *region, const char *name);

int sotc_region_append_state(struct sotc_region *r, struct sotc_state *state);

int sotc_region_serialize(struct sotc_region *region, json_object *state,
                         json_object **out);

int sotc_region_deserialize(json_object *j_r, struct sotc_state *state,
                            struct sotc_region **out);

int sotc_region_set_height(struct sotc_region *r, double h);
int sotc_region_get_height(struct sotc_region *r, double *h);

/* State api */

int sotc_add_state(struct sotc_region *region, const char *name,
                    struct sotc_state **out);

int sotc_add_exit(struct sotc_entry_exit **exit_p,
                 struct sotc_state *state,
                 const char *action_name);

int sotc_add_entry(struct sotc_entry_exit **entry_p,
                  struct sotc_state *state,
                  const char *action_name);

int sotc_add_transition(struct sotc_transition **transition,
                       struct sotc_state *source,
                       struct sotc_state *dest);

int sotc_state_append_region(struct sotc_state *state, struct sotc_region *r);

int sotc_state_serialize(struct sotc_state *state, json_object *region,
                        json_object **out);

int sotc_state_deserialize(json_object *j_state, struct sotc_region *region,
                          struct sotc_state **out);


int sotc_state_set_size(struct sotc_state *s, int x, int y);
int sotc_state_set_xy(struct sotc_state *s, int x, int y);

int sotc_state_get_size(struct sotc_state *s, int *x, int *y);
int sotc_state_get_xy(struct sotc_state *s, int *x, int *y);

const char * sotc_model_name(struct sotc_model *model);

#endif  // INCLUDE_SOTC_MODEL_H_
