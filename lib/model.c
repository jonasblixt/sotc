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

static int push_r_js_pair(struct tcm_stack *stack,
                          struct tcm_region *r,
                          json_object *js)
{
    int rc;

    rc = tcm_stack_push(stack, r);

    if (rc != TCM_OK)
        return rc;

    return tcm_stack_push(stack, js);
}

static int pop_r_js_pair(struct tcm_stack *stack,
                          struct tcm_region **r,
                          json_object **js)
{
    int rc;

    rc = tcm_stack_pop(stack, (void **) js);

    if (rc != TCM_OK)
        return rc;

    return tcm_stack_pop(stack, (void **) r);
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
    struct tcm_state *state = NULL;

    /* Allocate temporary stack for parsing */
    rc = tcm_stack_init(&stack, TCM_MAX_R_S);

    if (rc != TCM_OK)
        return rc;

    /* Populate stack with the root region */
    push_jr_s_pair(stack, j_region, NULL);

    while (pop_jr_s_pair(stack, &j_r, &state) == TCM_OK)
    {
        L_DEBUG("Parsing region");

        rc = tcm_region_deserialize(j_r, state, &r);

        if (rc != TCM_OK)
        {
            L_ERR("Could not de-serialize region");
            goto err_parse_error;
        }

        tcm_state_append_region(state, r);

        if (!model->root)
        {
            model->root = r;
        }

        L_DEBUG("Initialized region: %s", r->name);

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
            struct tcm_state *new_state;

            rc = tcm_state_deserialize(j_state, r, &new_state);

            if (rc != TCM_OK)
            {
                L_ERR("Could not de-serialize state");
                goto err_parse_error;
            }

            tcm_region_append_state(r, new_state);

            if (!json_object_object_get_ex(j_state, "region", &j_state_regions))
            {
                L_ERR("State '%s' no has regions", new_state->name);
                rc = -TCM_ERR_PARSE;
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

                if (rc != TCM_OK)
                {
                    L_ERR("Could not push to stack, aborting");
                    rc = -TCM_ERR_PARSE;
                    goto err_parse_error;
                }
            }
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
    struct tcm_stack *stack;
    struct tcm_state *s;
    struct tcm_region *r, *r2;
    json_object *current_j_state = NULL;
    json_object *parent_j_state = NULL;
    json_object *current_j_region = NULL;
    json_object *root_j_region = NULL;
    int rc = TCM_OK;

    L_DEBUG("Write model to %s", filename);

    rc = tcm_stack_init(&stack, TCM_MAX_R_S);

    if (rc != TCM_OK)
        return rc;

    /* Serialize the root region */
    rc = push_r_js_pair(stack, model->root, NULL);

    if (rc != TCM_OK)
        return rc;

    while (pop_r_js_pair(stack, &r, &parent_j_state) == TCM_OK)
    {
        L_DEBUG("Found region '%s' <%p> <%p>", r->name, r, parent_j_state);

        rc = tcm_region_serialize(r, parent_j_state, &current_j_region);

        if (rc != TCM_OK)
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

            rc = tcm_state_serialize(s, current_j_region, &current_j_state);

            /* Queue up sub-regions */
            for (r2 = s->regions; r2; r2 = r2->next)
                push_r_js_pair(stack, r2, current_j_state);
        }

    }

    if (rc != TCM_OK)
    {
        L_ERR("Serialization failed");
        goto err_free_out;
    }

    /* Create the model root object */

    json_object *jr = json_object_new_object();
    json_object *jr_kind = json_object_new_string("TCM Model");
    json_object *jr_version = json_object_new_int(model->version);
    json_object *jr_name = json_object_new_string(model->name);

    json_object_object_add(jr, "kind", jr_kind);
    json_object_object_add(jr, "version", jr_version);
    json_object_object_add(jr, "name", jr_name);
    json_object_object_add(jr, "region", root_j_region);


    const char *output_string = json_object_to_json_string_ext(jr,
                                                    JSON_C_TO_STRING_PRETTY);

    FILE *fp = fopen(filename, "w");

    if (!fp)
    {
        L_ERR("Could not open '%s' for writing", filename);
        rc = -TCM_ERR_IO;
        goto err_free_out;
    }

    size_t written_bytes = fwrite(output_string, 1, strlen(output_string), fp);

    if (written_bytes != strlen(output_string))
    {
        L_ERR("Write error to '%s'", filename);
        rc = -TCM_ERR_IO;
        goto err_close_fp_out;
    }

    L_INFO("Successfuly wrote %zu bytes to '%s'", written_bytes, filename);
err_close_fp_out:
    fclose(fp);
err_free_out:
    json_object_put(jr);
    tcm_stack_free(stack);
    return rc;
}

int tcm_model_free(struct tcm_model *model)
{
    struct tcm_stack *stack, *free_stack;
    struct tcm_state *s;
    struct tcm_region *r, *r2;
    int rc;

    rc = tcm_stack_init(&stack, TCM_MAX_R_S);

    if (rc != TCM_OK)
        return rc;

    rc = tcm_stack_init(&free_stack, TCM_MAX_OBJECTS);

    if (rc != TCM_OK)
        return rc;

    tcm_stack_push(stack, model->root);

    while (tcm_stack_pop(stack, (void **) &r) == TCM_OK)
    {
        L_DEBUG("loop %p", r->state);
        tcm_stack_push(free_stack, r);
        if (r->name)
        {
            free((void *) r->name);
        }

        for (s = r->state; s; s = s->next)
        {
            L_DEBUG("Found state '%s'", s->name);
            tcm_stack_push(free_stack, s);
            free((void *) s->name);

            for (r2 = s->regions; r2; r2 = r2->next)
            {
                L_DEBUG("Found region '%s'", r2->name);
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
