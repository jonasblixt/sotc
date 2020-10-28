#include <stdio.h>
#include <string.h>
#include <sotc/sotc.h>
#include <sotc/model.h>
#include <sotc/stack.h>
#include <json.h>

static int push_jr_s_pair(struct sotc_stack *stack,
                          json_object *jr,
                          struct sotc_state *state)
{
    int rc;

    rc = sotc_stack_push(stack, jr);

    if (rc != SOTC_OK)
        return rc;

    return sotc_stack_push(stack, state);
}

static int pop_jr_s_pair(struct sotc_stack *stack,
                          json_object **jr,
                          struct sotc_state **state)
{
    int rc;

    rc = sotc_stack_pop(stack, (void **) state);

    if (rc != SOTC_OK)
        return rc;

    return sotc_stack_pop(stack, (void **) jr);
}

static int push_r_js_pair(struct sotc_stack *stack,
                          struct sotc_region *r,
                          json_object *js)
{
    int rc;

    rc = sotc_stack_push(stack, r);

    if (rc != SOTC_OK)
        return rc;

    return sotc_stack_push(stack, js);
}

static int pop_r_js_pair(struct sotc_stack *stack,
                          struct sotc_region **r,
                          json_object **js)
{
    int rc;

    rc = sotc_stack_pop(stack, (void **) js);

    if (rc != SOTC_OK)
        return rc;

    return sotc_stack_pop(stack, (void **) r);
}

static int parse_action_list(json_object *j_list, struct sotc_action **out)
{
    size_t no_of_actions = json_object_array_length(j_list);
    json_object *j_action;
    json_object *j_name;
    json_object *j_id;

    L_DEBUG("no_of_actions = %zu", no_of_actions);

    struct sotc_action *list = NULL, *next;
    struct sotc_action *action;

    if (no_of_actions == 0) {
        (*out) = NULL;
        return SOTC_OK;
    }

    for (int n = 0; n < no_of_actions; n++) {
        j_action = json_object_array_get_idx(j_list, n);

        if (!json_object_object_get_ex(j_action, "name", &j_name)) {
            L_ERR("Could not read name property\n");
            return -SOTC_ERROR;
        }

        if (!json_object_object_get_ex(j_action, "id", &j_id)) {
            L_ERR("Could not read id property\n");
            return -SOTC_ERROR;
        }

        action = malloc(sizeof(*action));

        if (action == NULL)
            return -SOTC_ERROR;
        memset(action, 0, sizeof(*action));

        action->name = strdup(json_object_get_string(j_name));
        uuid_parse(json_object_get_string(j_id), action->id);

        L_DEBUG("Loaded action function '%s' %s", action->name,
                        json_object_get_string(j_id));

        if (list == NULL) {
            list = action;
            next = action;
        } else {
            next->next = action;
            next = next->next;
        }
    }

    (*out) = list;

    return SOTC_OK;
}


static int parse_triggers(json_object *j_list, struct sotc_model *model)
{
    size_t no_of_triggers = json_object_array_length(j_list);
    json_object *j_trigger;
    json_object *j_name;
    json_object *j_id;

    L_DEBUG("no_of_triggers = %zu", no_of_triggers);

    struct sotc_trigger *list = NULL, *next;
    struct sotc_trigger *trigger;

    if (no_of_triggers == 0) {
        model->triggers = NULL;
        return SOTC_OK;
    }

    for (int n = 0; n < no_of_triggers; n++) {
        j_trigger = json_object_array_get_idx(j_list, n);

        if (!json_object_object_get_ex(j_trigger, "name", &j_name)) {
            L_ERR("Could not read name property\n");
            return -SOTC_ERROR;
        }

        if (!json_object_object_get_ex(j_trigger, "id", &j_id)) {
            L_ERR("Could not read id property\n");
            return -SOTC_ERROR;
        }

        trigger = malloc(sizeof(*trigger));

        if (trigger == NULL)
            return -SOTC_ERROR;

        memset(trigger, 0, sizeof(*trigger));

        trigger->name = strdup(json_object_get_string(j_name));
        uuid_parse(json_object_get_string(j_id), trigger->id);

        L_DEBUG("Loaded trigger '%s' %s", trigger->name,
                                          json_object_get_string(j_id));

        if (list == NULL) {
            list = trigger;
            next = trigger;
        } else {
            next->next = trigger;
            next = next->next;
        }
    }

    model->triggers = list;

    return SOTC_OK;
}

static int parse_root_region(struct sotc_model *model, json_object *j_region)
{
    int rc = SOTC_OK;
    json_object *jobj;
    struct sotc_region *r = NULL;
    struct sotc_region *prev_region = NULL;
    struct json_object *j_r = NULL;
    struct sotc_stack *stack;
    struct sotc_state *parent_state = NULL;
    struct sotc_state *state = NULL;

    /* Allocate temporary stack for parsing */
    rc = sotc_stack_init(&stack, SOTC_MAX_R_S);

    if (rc != SOTC_OK)
        return rc;

    L_DEBUG("Pass 1...");
    /* Pass 1: Populate stack with the root region */
    push_jr_s_pair(stack, j_region, NULL);

    while (pop_jr_s_pair(stack, &j_r, &state) == SOTC_OK)
    {
        L_DEBUG("Parsing region");

        rc = sotc_region_deserialize(j_r, state, &r);

        if (rc != SOTC_OK)
        {
            L_ERR("Could not de-serialize region");
            goto err_parse_error;
        }

        sotc_state_append_region(state, r);

        if (!model->root)
        {
            model->root = r;
        }

        L_DEBUG("Initialized region: %s ps=%s", r->name,
                (r->parent_state != NULL)?(r->parent_state->name):"");

        if (!json_object_object_get_ex(j_r, "states", &jobj))
        {
            L_DEBUG("No states found in region");
            continue;
        }

        size_t n_elements = json_object_array_length(jobj);

        L_DEBUG("Array elements = %zu", n_elements);

        /* Iterate over all states in region 'j_r' */
        for (int i = 0; i < n_elements; i++)
        {
            json_object *j_state = json_object_array_get_idx(jobj, i);
            json_object *j_state_regions = NULL;
            struct sotc_state *new_state;

            rc = sotc_state_deserialize(model, j_state, r, &new_state);

            if (rc != SOTC_OK)
            {
                L_ERR("Could not de-serialize state");
                goto err_parse_error;
            }

            sotc_region_append_state(r, new_state);

            if (!json_object_object_get_ex(j_state, "region", &j_state_regions))
            {
                L_ERR("State '%s' no has regions", new_state->name);
                rc = -SOTC_ERR_PARSE;
                goto err_parse_error;

            }

            L_DEBUG("Looking for regions in state '%s'", new_state->name);

            size_t n_regions = json_object_array_length(j_state_regions);

            /* Iterate over all regions in state*/
            for (int n = 0; n < n_regions; n++)
            {
                L_DEBUG("Push jr_s_pair");
                rc = push_jr_s_pair(stack,
                     json_object_array_get_idx(j_state_regions, n), new_state);

                if (rc != SOTC_OK)
                {
                    L_ERR("Could not push to stack, aborting");
                    rc = -SOTC_ERR_PARSE;
                    goto err_parse_error;
                }
            }
        }
    }

    L_DEBUG("Pass 2...");
    /* Pass 2: Parse all transitions */
    sotc_stack_push(stack, (void *) j_region);

    while (sotc_stack_pop(stack, (void **) &j_r) == SOTC_OK)
    {
        L_DEBUG("Parsing region");

        if (!json_object_object_get_ex(j_r, "states", &jobj))
        {
            L_DEBUG("No states found in region");
            continue;
        }

        size_t n_elements = json_object_array_length(jobj);

        L_DEBUG("Array elements = %zu", n_elements);

        /* Iterate over all states in region 'j_r' */
        for (int i = 0; i < n_elements; i++)
        {
            json_object *j_state = json_object_array_get_idx(jobj, i);
            json_object *j_state_regions = NULL;
            json_object *j_transitions;
            json_object *j_state_id;
            uuid_t state_uu;
            struct sotc_state *state;

            json_object_object_get_ex(j_state, "id", &j_state_id);
            L_DEBUG("Pass 2: %s", json_object_get_string(j_state_id));
            uuid_parse(json_object_get_string(j_state_id), state_uu);

            if (json_object_object_get_ex(j_state, "transitions", &j_transitions)) {
                state = sotc_model_get_state_from_uuid(model, state_uu);
                if (state == NULL) {
                    L_ERR("Could not get state");
                    rc = -SOTC_ERROR;
                    goto err_parse_error;
                }

                rc = sotc_transition_deserialize(model, state, j_transitions);

                if (rc != SOTC_OK) {
                    L_ERR("Could not de-serialize transition");
                    goto err_parse_error;
                }
            }

            if (!json_object_object_get_ex(j_state, "region", &j_state_regions))
            {
                L_DEBUG("No regions found in state");
                continue;
            }

            size_t n_regions = json_object_array_length(j_state_regions);

            /* Iterate over all regions in state*/
            for (int n = 0; n < n_regions; n++)
            {
                rc = sotc_stack_push(stack,
                    (void *) json_object_array_get_idx(j_state_regions, n));

                if (rc != SOTC_OK)
                {
                    L_ERR("Could not push to stack, aborting");
                    rc = -SOTC_ERR_PARSE;
                    goto err_parse_error;
                }
            }
        }
    }

    L_DEBUG("Parse end");

err_parse_error:
    sotc_stack_free(stack);
    return rc;
}

static int sotc_model_parse(struct sotc_model *model)
{
    bool found_kind = false;
    bool found_version = false;
    bool found_region = false;
    bool found_name = false;
    int rc;

    json_object_object_foreach(model->jroot, key, val)
    {
        L_DEBUG("%s", key);
        if (strcmp(key, "name") == 0)
        {
            model->name = json_object_get_string(val);
            found_name = true;
        }
        else if (strcmp(key, "version") == 0)
        {
            model->version = json_object_get_int64(val);
            found_version = true;
        }
        else if (strcmp(key, "kind") == 0)
        {
            if (strcmp(json_object_get_string(val), "SOTC Model") == 0)
            {
                found_kind = true;
            }
            else
            {
                L_ERR("Unknown model kind (%s)", json_object_get_string(val));
            }
        }
        else if (strcmp(key, "entries") == 0)
        {
            rc = parse_action_list(val, &model->entries);
        }
        else if (strcmp(key, "exits") == 0)
        {
            rc = parse_action_list(val, &model->exits);
        }
        else if (strcmp(key, "actions") == 0)
        {
            rc = parse_action_list(val, &model->actions);
        }
        else if (strcmp(key, "guards") == 0)
        {
            rc = parse_action_list(val, &model->guards);
        }
        else if (strcmp(key, "triggers") == 0)
        {
            rc = parse_triggers(val, model);
        }
        else if (strcmp(key, "region") == 0)
        {
            found_region = true;
            /* Process region*/
            rc = parse_root_region(model, val);

            if (rc != SOTC_OK)
                break;
        }
        else
        {
            L_INFO("Found unknown data in root (%s)", key);
        }
    }

    if (found_region && found_kind && found_version && found_name)
        return rc;
    else
        return -SOTC_ERROR;
}

int sotc_model_load(const char *filename, struct sotc_model **model_pp)
{
    int rc = -SOTC_ERROR;
    struct sotc_model *model = (*model_pp);

    L_INFO("Loading model %s...", filename);

    model = malloc(sizeof(struct sotc_model));

    if (!model)
    {
        L_ERR("Could not allocate memory for model");
        return -SOTC_ERR_MEM;
    }

    memset(model, 0, sizeof(*model));
    (*model_pp) = model;

    FILE *fp = fopen(filename, "r");

    if (!fp)
    {
        L_ERR("Could not open '%s'", filename);
        rc = -SOTC_ERR_IO;
        goto err_free_model;
    }

    fseek(fp, 0, SEEK_END);
    size_t file_size_bytes = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *raw_json_data = malloc(file_size_bytes + 1);

    if (!raw_json_data)
    {
        L_ERR("Could not allocate memory for json data");
        rc = -SOTC_ERR_MEM;
        goto err_close_fp;
    }

    size_t read_bytes = fread(raw_json_data, 1, file_size_bytes, fp);

    if (read_bytes != file_size_bytes)
    {
        L_ERR("Could not read file (%s)", filename);
        rc = -SOTC_ERR_IO;
        goto err_free_json_data;
    }

    L_DEBUG("Model data loaded (%zu bytes), parsing JSON...",
                    file_size_bytes);

    enum json_tokener_error jerr;
    struct json_tokener *tok = json_tokener_new();

    if (!tok)
    {
        L_ERR("Could not allocate json tokener");
        rc = -SOTC_ERR_MEM;
        goto err_free_json_data;
    }

    model->jroot = json_tokener_parse_ex(tok, raw_json_data, file_size_bytes);

    if (tok->err != json_tokener_success)
    {
        L_ERR("Parse error (%s)", json_tokener_error_desc(tok->err));
        rc = -SOTC_ERROR;
        goto err_free_json_tok;
    }

    L_DEBUG("Successfuly parsed json model");

    json_tokener_free(tok);
    tok = NULL;
    free(raw_json_data);
    raw_json_data = NULL;
    fclose(fp);
    fp = NULL;

    rc = sotc_model_parse(model);

    if (rc != SOTC_OK)
    {
        L_ERR("Parse error");
        goto err_free_model;
    }

    return SOTC_OK;

err_free_json_tok:
    if (tok) {
        L_DEBUG("Freeing json tokenizer");
        json_tokener_free(tok);
    }
err_free_json_data:
    if (raw_json_data) {
        L_DEBUG("Freeing raw_json_data");
        free(raw_json_data);
    }
err_close_fp:
    if (fp) {
        L_DEBUG("Closing fd");
        fclose(fp);
    }
err_free_model:
    L_DEBUG("Free model");
    sotc_model_free(model);
    return rc;
}

static int serialize_action_list(struct sotc_action *list,
                                 json_object **out)
{
    struct sotc_action *item = list;
    char uuid_str[37];

    json_object *actions = json_object_new_array();
    json_object *action;
    json_object *name;
    json_object *id;

    while (item) {
        action = json_object_new_object();
        name = json_object_new_string(item->name);
        uuid_unparse(item->id, uuid_str);
        id = json_object_new_string(uuid_str);

        json_object_object_add(action, "name", name);
        json_object_object_add(action, "id", id);
        json_object_array_add(actions, action);
        item = item->next;
    }

    (*out) = actions;
    return SOTC_OK;
}

static int serialize_trigger_list(struct sotc_model *model,
                                 json_object **out)
{
    struct sotc_trigger *item = model->triggers;
    char uuid_str[37];

    json_object *triggers = json_object_new_array();
    json_object *trigger;
    json_object *name;
    json_object *id;

    while (item) {
        trigger = json_object_new_object();
        name = json_object_new_string(item->name);
        uuid_unparse(item->id, uuid_str);
        id = json_object_new_string(uuid_str);

        json_object_object_add(trigger, "name", name);
        json_object_object_add(trigger, "id", id);
        json_object_array_add(triggers, trigger);
        item = item->next;
    }

    (*out) = triggers;
    return SOTC_OK;
}

int sotc_model_create(struct sotc_model **model_pp, const char *name)
{
    struct sotc_model *model = NULL;

    model = malloc(sizeof(struct sotc_model));

    if (!model)
    {
        L_ERR("Could not allocate memory for model");
        return -SOTC_ERR_MEM;
    }

    memset(model, 0, sizeof(*model));
    (*model_pp) = model;
    model->name = name;

    model->root = malloc(sizeof(struct sotc_region));
    memset(model->root, 0, sizeof(*model->root));

    L_DEBUG("Created model '%s'", name);

    return SOTC_OK;
}

int sotc_model_write(const char *filename, struct sotc_model *model)
{
    struct sotc_stack *stack;
    struct sotc_state *s;
    struct sotc_region *r, *r2;
    json_object *current_j_state = NULL;
    json_object *parent_j_state = NULL;
    json_object *current_j_region = NULL;
    json_object *root_j_region = NULL;
    int rc = SOTC_OK;

    L_DEBUG("Write model to %s", filename);

    rc = sotc_stack_init(&stack, SOTC_MAX_R_S);

    if (rc != SOTC_OK)
        return rc;

    /* Serialize the root region */
    rc = push_r_js_pair(stack, model->root, NULL);

    if (rc != SOTC_OK)
        return rc;

    while (pop_r_js_pair(stack, &r, &parent_j_state) == SOTC_OK)
    {
        L_DEBUG("Found region '%s' <%p> <%p>", r->name, r, parent_j_state);

        rc = sotc_region_serialize(r, parent_j_state, &current_j_region);

        if (rc != SOTC_OK)
        {
            L_ERR("Could not serialize object");
            break;
        }

        if (!parent_j_state)
        {
            root_j_region = current_j_region;
        }

        for (s = r->state; s; s = s->next)
        {
            L_DEBUG("Found state '%s'", s->name);

            rc = sotc_state_serialize(s, current_j_region, &current_j_state);

            /* Queue up sub-regions */
            for (r2 = s->regions; r2; r2 = r2->next)
                push_r_js_pair(stack, r2, current_j_state);
        }

    }

    if (rc != SOTC_OK)
    {
        L_ERR("Serialization failed");
        goto err_free_out;
    }

    /* Serialize actions */
    json_object *j_actions;
    json_object *j_entries;
    json_object *j_exits;
    json_object *j_guards;

    rc = serialize_action_list(model->actions, &j_actions);

    if (rc != SOTC_OK)
    {
        L_ERR("Actions serialization failed");
        goto err_free_out;
    }

    rc = serialize_action_list(model->entries, &j_entries);

    if (rc != SOTC_OK)
    {
        L_ERR("Entries serialization failed");
        goto err_free_out;
    }

    rc = serialize_action_list(model->exits, &j_exits);

    if (rc != SOTC_OK)
    {
        L_ERR("Exits serialization failed");
        goto err_free_out;
    }

    rc = serialize_action_list(model->guards, &j_guards);

    if (rc != SOTC_OK)
    {
        L_ERR("Guards serialization failed");
        goto err_free_out;
    }

    /* Serialize triggers */
    json_object *j_triggers;

    rc = serialize_trigger_list(model, &j_triggers);

    if (rc != SOTC_OK)
    {
        L_ERR("Triggers serialization failed");
        goto err_free_out;
    }

    /* Create the model root object */

    json_object *jr = json_object_new_object();
    json_object *jr_kind = json_object_new_string("SOTC Model");
    json_object *jr_version = json_object_new_int(model->version);
    json_object *jr_name = json_object_new_string(model->name);

    json_object_object_add(jr, "kind", jr_kind);
    json_object_object_add(jr, "version", jr_version);
    json_object_object_add(jr, "name", jr_name);
    json_object_object_add(jr, "triggers", j_triggers);
    json_object_object_add(jr, "actions", j_actions);
    json_object_object_add(jr, "entries", j_entries);
    json_object_object_add(jr, "exits", j_exits);
    json_object_object_add(jr, "guards", j_guards);
    json_object_object_add(jr, "region", root_j_region);


    const char *output_string = json_object_to_json_string_ext(jr,
                                                    JSON_C_TO_STRING_PRETTY);

    FILE *fp = fopen(filename, "w");

    if (!fp)
    {
        L_ERR("Could not open '%s' for writing", filename);
        rc = -SOTC_ERR_IO;
        goto err_free_out;
    }

    size_t written_bytes = fwrite(output_string, 1, strlen(output_string), fp);

    if (written_bytes != strlen(output_string))
    {
        L_ERR("Write error to '%s'", filename);
        rc = -SOTC_ERR_IO;
        goto err_close_fp_out;
    }

    L_INFO("Successfuly wrote %zu bytes to '%s'", written_bytes, filename);
err_close_fp_out:
    fclose(fp);
err_free_out:
    json_object_put(jr);
    sotc_stack_free(stack);
    return rc;
}

static int free_action_list(struct sotc_action *list)
{
    struct sotc_action *item = list;
    struct sotc_action *tmp;

    if (list == NULL)
        return SOTC_OK;

    while (item) {
        tmp = item->next;
        free((void *) item->name);
        free(item);
        item = tmp;
    }

    return SOTC_OK;
}

int free_action_ref_list(struct sotc_action_ref *list)
{
    struct sotc_action_ref *item = list;
    struct sotc_action_ref *tmp;

    if (list == NULL)
        return SOTC_OK;

    while (item) {
        tmp = item->next;
        free(item);
        item = tmp;
    }

    return SOTC_OK;
}

static int free_triggers(struct sotc_model *model)
{
    struct sotc_trigger *item = model->triggers;
    struct sotc_trigger *tmp;

    if (item == NULL)
        return SOTC_OK;

    while (item) {
        tmp = item->next;
        free((void *) item->name);
        free(item);
        item = tmp;
    }

    return SOTC_OK;
}

int sotc_model_free(struct sotc_model *model)
{
    struct sotc_stack *stack, *free_stack;
    struct sotc_state *s;
    struct sotc_region *r, *r2;
    int rc;

    rc = sotc_stack_init(&stack, SOTC_MAX_R_S);

    if (rc != SOTC_OK)
        return rc;

    rc = sotc_stack_init(&free_stack, SOTC_MAX_OBJECTS);

    if (rc != SOTC_OK)
        return rc;

    L_DEBUG("Freeing actions");
    rc = free_action_list(model->actions);

    if (rc != SOTC_OK)
        return rc;

    L_DEBUG("Freeing entries");
    rc = free_action_list(model->entries);

    if (rc != SOTC_OK)
        return rc;

    L_DEBUG("Freeing exits");
    rc = free_action_list(model->exits);

    if (rc != SOTC_OK)
        return rc;

    L_DEBUG("Freeing guards");
    rc = free_action_list(model->guards);

    if (rc != SOTC_OK)
        return rc;

    L_DEBUG("Freeing triggers");
    rc = free_triggers(model);

    if (rc != SOTC_OK)
        return rc;

    sotc_stack_push(stack, model->root);

    while (sotc_stack_pop(stack, (void **) &r) == SOTC_OK)
    {
        L_DEBUG("loop %p", r->state);
        sotc_stack_push(free_stack, r);
        if (r->name)
        {
            free((void *) r->name);
        }

        for (s = r->state; s; s = s->next)
        {
            L_DEBUG("Found state '%s'", s->name);
            sotc_stack_push(free_stack, s);
            free_action_ref_list(s->entries);
            free_action_ref_list(s->exits);
            sotc_transition_free(s->transition);
            free((void *) s->name);

            for (r2 = s->regions; r2; r2 = r2->next)
            {
                L_DEBUG("Found region '%s'", r2->name);
                sotc_stack_push(stack, (void *) r2);
            }
        }
    }

    sotc_stack_free(stack);

    void *p;

    while (sotc_stack_pop(free_stack, &p) == SOTC_OK)
    {
        L_DEBUG("Free <%p>", p);
        free(p);
    }

    sotc_stack_free(free_stack);

    if (model->jroot)
    {
        json_object_put(model->jroot);
    }

    free(model);
    return SOTC_OK;
}

const char * sotc_model_name(struct sotc_model *model)
{
    return model->name;
}

int sotc_model_add_action(struct sotc_model *model,
                          enum sotc_action_kind kind,
                          const char *name,
                          struct sotc_action **act)
{
    int rc = SOTC_OK;
    struct sotc_action *action;
    struct sotc_action *list, **dest;

    action = malloc(sizeof(struct sotc_action));

    if (action == NULL)
        return -1;

    memset(action, 0, sizeof(*action));

    uuid_generate_random(action->id);
    action->kind = kind;
    action->name = strdup(name);

    switch (kind) {
        case SOTC_ACTION_ACTION:
            dest = &model->actions;
        break;
        case SOTC_ACTION_ENTRY:
            dest = &model->entries;
        break;
        case SOTC_ACTION_EXIT:
            dest = &model->exits;
        break;
        case SOTC_ACTION_GUARD:
            dest = &model->guards;
        break;
        default:
            rc = -1;
            goto err_free_out;
    }

    list = *dest;

    if (list == NULL) {
        list = action;
        (*dest) = list;
    } else {
        while (list->next != NULL)
            list = list->next;
        list->next = action;
    }

    if (act) {
        (*act) = action;
    }

    return rc;
err_free_out:
    free((void *) action->name);
    free(action);
    return rc;
}

static int action_list_delete(struct sotc_action **list_in, uuid_t id)
{
    bool found_item = false;
    struct sotc_action *list = *list_in;
    struct sotc_action *item = list;
    struct sotc_action *prev, *next;

    prev = NULL;

    while (item) {
        next = item->next;

        if (uuid_compare(item->id, id) == 0) {
            if (prev)
                prev->next = next;

            /* If this is the first and only item, set input to NULL */
            if (item == *list_in)
                (*list_in) = NULL;

            free((void *) item->name);
            memset(item, 0, sizeof(*item));
            free(item);
            found_item = true;
        }

        prev = item;
        item = item->next;
    }

    if (found_item)
        return SOTC_OK;
    else
        return -SOTC_ERROR;
}

int sotc_model_delete_action(struct sotc_model *model, uuid_t id)
{
    uuid_t id_tmp;
    char uuid_str[37];

    memcpy(id_tmp, id, 16);
    uuid_unparse(id, uuid_str);
    L_DEBUG("Deleting action %s", uuid_str);

    if (action_list_delete(&model->entries, id_tmp) == SOTC_OK)
        return SOTC_OK;
    if (action_list_delete(&model->exits, id_tmp) == SOTC_OK)
        return SOTC_OK;
    if (action_list_delete(&model->guards, id_tmp) == SOTC_OK)
        return SOTC_OK;
    if (action_list_delete(&model->actions, id_tmp) == SOTC_OK)
        return SOTC_OK;
    return -SOTC_ERROR;
}

struct sotc_action* sotc_model_get_entries(struct sotc_model *model)
{
    return model->entries;
}

struct sotc_action* sotc_model_get_exits(struct sotc_model *model)
{
    return model->exits;
}

struct sotc_action* sotc_model_get_guards(struct sotc_model *model)
{
    return model->guards;
}

struct sotc_action* sotc_model_get_actions(struct sotc_model *model)
{
    return model->actions;
}

int sotc_model_get_action(struct sotc_model *model, uuid_t id,
                          enum sotc_action_kind kind,
                          struct sotc_action **result)
{
    struct sotc_action *list = NULL;

    switch (kind) {
        case SOTC_ACTION_ACTION:
            list = model->actions;
        break;
        case SOTC_ACTION_GUARD:
            list = model->guards;
        break;
        case SOTC_ACTION_ENTRY:
            list = model->entries;
        break;
        case SOTC_ACTION_EXIT:
            list = model->exits;
        break;
        default:
            return -SOTC_ERROR;
    }

    while (list) {
        if (uuid_compare(id, list->id) == 0) {
            (*result) = list;
            return SOTC_OK;
        }

        list = list->next;
    }

    return -SOTC_ERROR;
}

int sotc_model_add_trigger(struct sotc_model *model, const char *name,
                           struct sotc_trigger **out)
{
    int rc = SOTC_OK;
    struct sotc_trigger *trigger;
    struct sotc_trigger *list, **dest;

    trigger = malloc(sizeof(struct sotc_trigger));

    if (trigger == NULL)
        return -SOTC_ERROR;

    memset(trigger, 0, sizeof(*trigger));

    uuid_generate_random(trigger->id);
    trigger->name = strdup(name);
    list = model->triggers;

    if (list == NULL) {
        list = trigger;
        model->triggers = list;
    } else {
        while (list->next != NULL)
            list = list->next;
        list->next = trigger;
    }

    if (out) {
        (*out) = trigger;
    }

    return rc;
err_free_out:
    free((void *) trigger->name);
    free(trigger);
    return rc;
}

int sotc_model_delete_trigger(struct sotc_model *model, uuid_t id)
{
    bool found_item = false;
    struct sotc_trigger *list = model->triggers;
    struct sotc_trigger *item = list;
    struct sotc_trigger *prev, *next;

    prev = NULL;

    while (item) {
        next = item->next;

        if (uuid_compare(item->id, id) == 0) {
            if (prev)
                prev->next = next;

            /* If this is the first and only item, set input to NULL */
            if (item == model->triggers)
                model->triggers = NULL;

            free((void *) item->name);
            memset(item, 0, sizeof(*item));
            free(item);
            found_item = true;
        }

        prev = item;
        item = item->next;
    }

    if (found_item)
        return SOTC_OK;
    else
        return -SOTC_ERROR;
}

int sotc_model_get_trigger(struct sotc_model *model, uuid_t id,
                           struct sotc_trigger **out)
{
    struct sotc_trigger *list = model->triggers;

    while (list) {
        if (uuid_compare(id, list->id) == 0) {
            (*out) = list;
            return SOTC_OK;
        }

        list = list->next;
    }

    return -SOTC_ERROR;
}

struct sotc_trigger* sotc_model_get_triggers(struct sotc_model *model)
{
    return model->triggers;
}

struct sotc_state *sotc_model_get_state_from_uuid(struct sotc_model *model,
                                                  uuid_t id)
{
    int rc;
    struct sotc_region *r, *r2;
    struct sotc_state *s;
    static struct sotc_stack *stack;

    rc = sotc_stack_init(&stack, SOTC_MAX_R_S);

    if (rc != SOTC_OK) {
        L_ERR("Could not init stack");
        return NULL;
    }

    rc = sotc_stack_push(stack, (void *) model->root);

    while (sotc_stack_pop(stack, (void *) &r) == SOTC_OK) {
        for (s = r->state; s; s = s->next) {
            if (uuid_compare(s->id, id) == 0) {
                goto search_out;
            }

            for (r2 = s->regions; r2; r2 = r2->next) {
                sotc_stack_push(stack, (void *) r2);
            }
        }
    }

search_out:
    sotc_stack_free(stack);
    return s;
}

struct sotc_trigger * sotc_model_get_trigger_from_uuid(struct sotc_model *model,
                                                       uuid_t id)
{
    struct sotc_trigger *list = model->triggers;

    while (list) {
        if (uuid_compare(list->id, id) == 0) {
            return list;
        }
        list = list->next;
    }

    return NULL;
}

int sotc_model_deserialize_coords(json_object *j_coords,
                            struct sotc_coords *coords)
{
    json_object *jobj;

    if (!json_object_object_get_ex(j_coords, "x", &jobj)) {
        L_ERR("Could not read x");
        return -SOTC_ERROR;
    }

    coords->x = json_object_get_double(jobj);

    if (!json_object_object_get_ex(j_coords, "y", &jobj)) {
        L_ERR("Could not read x");
        return -SOTC_ERROR;
    }

    coords->y = json_object_get_double(jobj);

    if (!json_object_object_get_ex(j_coords, "w", &jobj)) {
        L_ERR("Could not read x");
        return -SOTC_ERROR;
    }

    coords->w = json_object_get_double(jobj);

    if (!json_object_object_get_ex(j_coords, "h", &jobj)) {
        L_ERR("Could not read x");
        return -SOTC_ERROR;
    }

    coords->h = json_object_get_double(jobj);

}
