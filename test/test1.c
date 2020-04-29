#include <string.h>
#include <tcm/tcm.h>
#include <tcm/model.h>
#include <json.h>
#include "nala.h"
#include "common.h"

TEST(version)
{
    printf("TCM Library version: %s\n", tcm_library_version());
}

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
