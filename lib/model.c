#include <stdio.h>
#include <string.h>
#include <tcm/tcm.h>
#include <tcm/model.h>

static int tcm_model_parse(struct tcm_model *model)
{
    return TCM_OK;
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

    L_DEBUG("Created model '%s'", name);

    return TCM_OK;
}

int tcm_model_write(const char *filename, struct tcm_model *model)
{
    return -TCM_ERROR;
}

int tcm_model_free(struct tcm_model *model)
{
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
