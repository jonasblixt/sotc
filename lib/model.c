#include <stdio.h>
#include <string.h>
#include <tcm/tcm.h>
#include <tcm/model.h>
#include <tcm/stack.h>
#include <json.h>

static int push_jr_s_pair(struct tcm_stack *stack,
                          json_object *jr,
                          struct tcm_state *state)
{
    int rc;

    rc = tcm_stack_push(stack, jr);

    if (rc != TCM_OK)
        return rc;

    return tcm_stack_push(stack, state);
}

static int pop_jr_s_pair(struct tcm_stack *stack,
                          json_object **jr,
                          struct tcm_state **state)
{
    int rc;

    rc = tcm_stack_pop(stack, (void **) state);

    if (rc != TCM_OK)
        return rc;

    return tcm_stack_pop(stack, (void **) jr);
}

static int parse_region(struct tcm_model *model, json_object *j_region)
{
    int rc = TCM_OK;
    json_object *jobj;
    struct tcm_region *r = NULL;
    struct tcm_region *prev_region = NULL;
    struct json_object *j_r = NULL;
    struct tcm_stack *stack;
    struct tcm_state *parent_state = NULL;
    struct tcm_state *prev_state = NULL;
    struct tcm_state *state = NULL;

    /* Allocate temporary stack for parsing */
    rc = tcm_stack_init(&stack, 1024);

    if (rc != TCM_OK)
        return rc;

    /* Populate stack with the root region */
    push_jr_s_pair(stack, j_region, NULL);

    while (pop_jr_s_pair(stack, &j_r, &state) == TCM_OK)
    {
        L_DEBUG("Parsing region");

        r = malloc(sizeof(struct tcm_region));

        if (!model->root)
        {
            model->root = r;
        }

        memset(r, 0, sizeof(*r));

        if (!json_object_object_get_ex(j_r, "name", &jobj))
        {
            L_INFO("Could not read name property");
            continue;
        }

        r->parent_state = state;
        r->name = json_object_get_string(jobj);

        if (state)
        {
            L_DEBUG("Belongs to %s", state->name);
            /* Place region last in the list of regions for this state */
            if (!state->regions)
            {
                state->regions = r;
            }
            else
            {
                struct tcm_region *p_r = state->regions;

                while (!p_r->next)
                    p_r = p_r->next;

                p_r->next = r;
            }
        }

        L_DEBUG("Initialized region: %s", r->name);

        if (!json_object_object_get_ex(j_r, "states", &jobj))
        {
            L_DEBUG("No states found in region");
        }

        size_t n_elements = json_object_array_length(jobj);

        L_DEBUG("Array elements = %zu", n_elements);
        prev_state = NULL;

        /* Iterate over all states in region 'j_r' */
        for (int i = 0; i < n_elements; i++)
        {
            json_object *j_state = json_object_array_get_idx(jobj, i);
            json_object *j_state_name;
            json_object *j_state_regions;

            state = malloc(sizeof(struct tcm_state));
            memset(state, 0 , sizeof(*state));

            if (prev_state)
            {
                prev_state->next = state;
            }


            if (!r->state)
            {
                r->state = state;
            }
            else
            {
                struct tcm_state *p_s = r->state;

                while (!p_s->next)
                    p_s = p_s->next;

                p_s->next = state;
            }

            if (!json_object_object_get_ex(j_state, "name", &j_state_name))
            {
                L_ERR("Missing name property, aborting");
                rc = -TCM_ERR_PARSE;
                goto err_parse_error;
            }

            state->name = json_object_get_string(j_state_name);
            state->parent_region = r;

            L_DEBUG("Loading state %s", state->name);

            if (!json_object_object_get_ex(j_state, "region", &j_state_regions))
            {
                L_ERR("State '%s' no has regions", state->name);
                rc = -TCM_ERR_PARSE;
                goto err_parse_error;

            }

            if (prev_state)
            {
                L_DEBUG("Linking %s to %s", prev_state->name,
                                            state->name);
            }

            size_t n_regions = json_object_array_length(j_state_regions);

            /* Iterate over all regions in state*/
            for (int n = 0; n < n_regions; n++)
            {
                L_DEBUG("Push jr_s_pair");
                rc = push_jr_s_pair(stack,
                        json_object_array_get_idx(j_state_regions, n), state);

                if (rc != TCM_OK)
                {
                    L_ERR("Could not push to stack, aborting");
                    rc = -TCM_ERR_PARSE;
                    goto err_parse_error;
                }
            }

            prev_state = state;
        }
    }

    L_DEBUG("Parse end");

err_parse_error:
    tcm_stack_free(stack);
    return rc;
}

static int tcm_model_parse(struct tcm_model *model)
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
            if (strcmp(json_object_get_string(val), "TCM Model") == 0)
            {
                found_kind = true;
            }
            else
            {
                L_ERR("Unknown model kind (%s)", json_object_get_string(val));
            }
        }
        else if (strcmp(key, "region") == 0)
        {
            found_region = true;
            /* Process region*/
            rc = parse_region(model, val);

            if (rc != TCM_OK)
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
        return -TCM_ERROR;
}

int tcm_model_load(const char *filename, struct tcm_model **model_pp)
{
    int rc = -TCM_ERROR;
    struct tcm_model *model = (*model_pp);

    L_INFO("Loading model %s...", filename);

    model = malloc(sizeof(struct tcm_model));

    if (!model)
    {
        L_ERR("Could not allocate memory for model");
        return -TCM_ERR_MEM;
    }

    memset(model, 0, sizeof(*model));
    (*model_pp) = model;

    FILE *fp = fopen(filename, "r");

    if (!fp)
    {
        L_ERR("Could not open '%s'", filename);
        rc = -TCM_ERR_IO;
        goto err_free_model;
    }

    fseek(fp, 0, SEEK_END);
    size_t file_size_bytes = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *raw_json_data = malloc(file_size_bytes + 1);

    if (!raw_json_data)
    {
        L_ERR("Could not allocate memory for json data");
        rc = -TCM_ERR_MEM;
        goto err_close_fp;
    }

    size_t read_bytes = fread(raw_json_data, 1, file_size_bytes, fp);

    if (read_bytes != file_size_bytes)
    {
        L_ERR("Could not read file (%s)", filename);
        rc = -TCM_ERR_IO;
        goto err_free_json_data;
    }

    L_DEBUG("Model data loaded (%zu bytes), parsing JSON...",
                    file_size_bytes);

    enum json_tokener_error jerr;
    struct json_tokener *tok = json_tokener_new();

    if (!tok)
    {
        L_ERR("Could not allocate json tokener");
        rc = -TCM_ERR_MEM;
        goto err_free_json_data;
    }

    model->jroot = json_tokener_parse_ex(tok, raw_json_data, file_size_bytes);

    if (tok->err != json_tokener_success)
    {
        L_ERR("Parse error (%s)", json_tokener_error_desc(tok->err));
        rc = -TCM_ERROR;
        goto err_free_json_tok;
    }

    L_DEBUG("Successfuly parsed json model");

    json_tokener_free(tok);
    free(raw_json_data);
    fclose(fp);

    rc = tcm_model_parse(model);

    if (rc != TCM_OK)
    {
        L_ERR("Parse error");
        goto err_free_json;
    }

    return TCM_OK;

err_free_json:
    json_object_put(model->jroot);
err_free_json_tok:
    json_tokener_free(tok);
err_free_json_data:
    free(raw_json_data);
err_close_fp:
    fclose(fp);
err_free_model:
    free(model);
    return rc;
}

int tcm_model_create(struct tcm_model **model_pp, const char *name)
{
    struct tcm_model *model = NULL;

    model = malloc(sizeof(struct tcm_model));

    if (!model)
    {
        L_ERR("Could not allocate memory for model");
        return -TCM_ERR_MEM;
    }

    memset(model, 0, sizeof(*model));
    (*model_pp) = model;
    model->name = name;

    model->root = malloc(sizeof(struct tcm_region));
    memset(model->root, 0, sizeof(*model->root));

    L_DEBUG("Created model '%s'", name);

    return TCM_OK;
}

int tcm_model_write(const char *filename, struct tcm_model *model)
{
    return -TCM_ERROR;
}

int tcm_model_free(struct tcm_model *model)
{
    struct tcm_stack *stack, *free_stack;
    struct tcm_state *s;
    struct tcm_region *r, *r2;
    int rc;

    rc = tcm_stack_init(&stack, 1024);

    if (rc != TCM_OK)
        return rc;

    rc = tcm_stack_init(&free_stack, 1024*1024);

    if (rc != TCM_OK)
        return rc;

    tcm_stack_push(stack, model->root);

    while (tcm_stack_pop(stack, (void **) &r) == TCM_OK)
    {
        L_DEBUG("loop %p", r->state);
        tcm_stack_push(free_stack, r);

        for (s = r->state; s; s = s->next)
        {
            L_DEBUG("Found state <%p>", s);
            tcm_stack_push(free_stack, s);

            for (r2 = s->regions; r2; r2 = r2->next)
            {
                L_DEBUG("Found region <%p>", r2);
                tcm_stack_push(stack, (void *) r2);
            }
        }
    }

    tcm_stack_free(stack);

    void *p;

    while (tcm_stack_pop(free_stack, &p) == TCM_OK)
    {
        L_DEBUG("Free <%p>", p);
        free(p);
    }

    tcm_stack_free(free_stack);

    if (model->jroot)
    {
        json_object_put(model->jroot);
    }

    free(model);
    return TCM_OK;
}

const char * tcm_model_name(struct tcm_model *model)
{
    return model->name;
}
