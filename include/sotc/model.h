#ifndef INCLUDE_SOTC_MODEL_H_
#define INCLUDE_SOTC_MODEL_H_

#include <stdint.h>
#include <uuid/uuid.h>
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

enum sotc_action_kind
{
    SOTC_ACTION_ACTION,
    SOTC_ACTION_GUARD,
    SOTC_ACTION_ENTRY,
    SOTC_ACTION_EXIT,
};

enum sotc_side
{
    SOTC_SIDE_LEFT,
    SOTC_SIDE_RIGHT,
    SOTC_SIDE_TOP,
    SOTC_SIDE_BOTTOM,
};

struct sotc_action
{
    uuid_t id;
    const char *name;
    enum sotc_action_kind kind;
    struct sotc_action *next;
};

struct sotc_action_ref
{
    struct sotc_action *act;
    struct sotc_action_ref *next;
};

struct sotc_trigger
{
    uuid_t id;
    const char *name;
    struct sotc_trigger *next;
};

struct sotc_vertice
{
    double x;
    double y;
    struct sotc_vertice *next;
};

struct sotc_transition_state_ref
{
    struct sotc_state *state;
    double offset;
    enum sotc_side side;
};

struct sotc_coords
{
    double x;
    double y;
    double w;
    double h;
};

struct sotc_transition_state_condition
{
    struct sotc_state *state;
    bool positive;
    struct sotc_transition_state_condition *next;
};

struct sotc_transition
{
    uuid_t id;
    struct sotc_trigger *trigger;
    enum sotc_transition_kind kind;
    struct sotc_action_ref *action;
    struct sotc_action_ref *guard;
    struct sotc_transition_state_ref source;
    struct sotc_transition_state_ref dest;
    struct sotc_coords text_block_coords;
    struct sotc_vertice *vertices;
    struct sotc_transition_state_condition *state_conditions;
    struct sotc_transition *next;
};

struct sotc_region
{
    uuid_t id;
    const char *name;
    bool off_page;
    double h;
    bool focus;
    struct sotc_state *state;
    struct sotc_state *parent_state;
    struct sotc_state *last_state;
    struct sotc_region *next;
};

struct sotc_state
{
    uuid_t id;
    const char *name;
    double x;
    double y;
    double w;
    double h;
    double region_y_offset;
    bool focus;
    enum sotc_state_kind kind;
    struct sotc_transition *transition;
    struct sotc_action_ref *entries;
    struct sotc_action_ref *exits;
    struct sotc_region *regions;
    struct sotc_region *parent_region;
    struct sotc_region *last_region;
    struct sotc_state *next;
};

struct sotc_model
{
    json_object *jroot;
    struct sotc_region *root;
    struct sotc_action *entries; /* Global list of entry functions */
    struct sotc_action *exits;   /* Global list of exit functions  */
    struct sotc_action *guards;  /* Global list of guard functions */
    struct sotc_action *actions; /* Global list of action functions */
    struct sotc_trigger *triggers;
    const char *name;
    int version;
};

int sotc_model_load(const char *filename, struct sotc_model **model);
int sotc_model_create(struct sotc_model **model, const char *name);
int sotc_model_write(const char *filename, struct sotc_model *model);
int sotc_model_free(struct sotc_model *model);
int sotc_model_add_action(struct sotc_model *model,
                          enum sotc_action_kind kind,
                          const char *name,
                          struct sotc_action **act);
int sotc_model_delete_action(struct sotc_model *model, uuid_t id);
int sotc_model_get_action(struct sotc_model *model, uuid_t id,
                          enum sotc_action_kind kind,
                          struct sotc_action **result);

int sotc_model_add_trigger(struct sotc_model *model, const char *name,
                           struct sotc_trigger **out);
int sotc_model_delete_trigger(struct sotc_model *model, uuid_t id);
int sotc_model_get_trigger(struct sotc_model *model, uuid_t id,
                           struct sotc_trigger **out);

int sotc_model_deserialize_coords(json_object *j_coords,
                                  struct sotc_coords *coords);

struct sotc_action* sotc_model_get_entries(struct sotc_model *model);
struct sotc_action* sotc_model_get_exits(struct sotc_model *model);
struct sotc_action* sotc_model_get_guards(struct sotc_model *model);
struct sotc_action* sotc_model_get_actions(struct sotc_model *model);
struct sotc_trigger* sotc_model_get_triggers(struct sotc_model *model);

struct sotc_state *sotc_model_get_state_from_uuid(struct sotc_model *model,
                                                  uuid_t id);

struct sotc_trigger * sotc_model_get_trigger_from_uuid(struct sotc_model *model,
                                                       uuid_t id);
int free_action_ref_list(struct sotc_action_ref *list);

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

int sotc_state_add_entry(struct sotc_model *model,
                         struct sotc_state *state,
                         uuid_t id);

int sotc_state_add_exit(struct sotc_model *model,
                        struct sotc_state *state,
                        uuid_t id);

int sotc_state_delete_entry(struct sotc_state *state, uuid_t id);
int sotc_state_delete_exit(struct sotc_state *state, uuid_t id);

int sotc_state_get_entries(struct sotc_state *state,
                           struct sotc_action_ref **list);
int sotc_state_get_exits(struct sotc_state *state,
                         struct sotc_action_ref **list);

int sotc_state_add_transition(struct sotc_state *source,
                              struct sotc_state *dest,
                              struct sotc_transition **transition);

int sotc_state_delete_transition(struct sotc_state *source,
                                 struct sotc_transition *transition);

int sotc_state_get_transitions(struct sotc_state *state,
                               struct sotc_transition **transitions);

int sotc_state_append_region(struct sotc_state *state, struct sotc_region *r);

int sotc_state_serialize(struct sotc_state *state, json_object *region,
                        json_object **out);

int sotc_state_deserialize(struct sotc_model *model,
                           json_object *j_state,
                           struct sotc_region *region,
                           struct sotc_state **out);

int sotc_state_set_size(struct sotc_state *s, double x, double y);
int sotc_state_set_xy(struct sotc_state *s, double x, double y);

int sotc_state_get_size(struct sotc_state *s, double *x, double *y);
int sotc_state_get_xy(struct sotc_state *s, double *x, double *y);


const char * sotc_model_name(struct sotc_model *model);

/* Transition API */
int sotc_transition_deserialize(struct sotc_model *model,
                                struct sotc_state *state,
                                json_object *j_object);

int sotc_transition_set_trigger(struct sotc_model *model,
                                struct sotc_transition *transition,
                                uuid_t id);

int sotc_transition_add_guard(struct sotc_model *model,
                              struct sotc_transition *transition,
                              uuid_t id);

int sotc_transition_delete_guard(struct sotc_transition *transition, uuid_t id);
struct sotc_action_ref *sotc_transition_get_guards(struct sotc_transition *t);

int sotc_transition_add_action(struct sotc_model *model,
                               struct sotc_transition *transition,
                               uuid_t id);

int sotc_transition_delete_action(struct sotc_transition *transition, uuid_t id);
struct sotc_action_ref *sotc_transition_get_actions(struct sotc_transition *t);

int sotc_transition_add_state_condition(struct sotc_model *model,
                                        struct sotc_transition *transition,
                                        uuid_t id,
                                        bool positive);

int sotc_transition_delete_state_condition(struct sotc_transition *transition,
                                            uuid_t id);

struct sotc_transition_state_condition *
sotc_transition_get_state_conditions(struct sotc_transition *t);

int sotc_transition_free(struct sotc_transition *transition);

#endif  // INCLUDE_SOTC_MODEL_H_
