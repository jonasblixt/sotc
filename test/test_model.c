#include <string.h>
#include <tcm/tcm.h>
#include <tcm/model.h>
#include <json.h>
#include "nala.h"
#include "common.h"

TEST(load_model)
{
    int rc;
    struct tcm_model *model;

    rc = tcm_model_load(TCM_TEST_SRC"test1.tcm", &model);
    ASSERT_EQ(rc, TCM_OK);

    printf("Model load %i\n", rc);

    json_object_object_foreach(model->jroot, key, val)
    {
        printf("%s: %s\n", key, json_object_get_string(val));
    }


    rc = tcm_model_free(model);
    ASSERT_EQ(rc, TCM_OK);
}


TEST(create_model)
{
    int rc;
    struct tcm_model *model;

    rc = tcm_model_create(&model, "Test");
    ASSERT_EQ(rc, TCM_OK);
    ASSERT(model != NULL);
    ASSERT_EQ((char *) tcm_model_name(model), "Test");
    rc = tcm_model_free(model);
    ASSERT_EQ(rc, TCM_OK);
}

TEST(load_model2)
{
    int rc;
    struct tcm_model *model;

    rc = tcm_model_load(TCM_TEST_SRC"test2.tcm", &model);
    ASSERT_EQ(rc, TCM_OK);

    printf("Model load %i\n", rc);

    json_object_object_foreach(model->jroot, key, val)
    {
        printf("%s: %s\n", key, json_object_get_string(val));
    }


    rc = tcm_model_free(model);
    ASSERT_EQ(rc, TCM_OK);
}
