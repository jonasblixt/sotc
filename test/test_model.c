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

TEST(load_empty_root)
{
    int rc;
    struct tcm_model *model;

    rc = tcm_model_load(TCM_TEST_SRC"test_empty_root.tcm", &model);
    ASSERT_EQ(rc, TCM_OK);

    printf("Model load %i\n", rc);

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

TEST(write_model)
{
    int rc;
    struct tcm_model *model;
    struct tcm_state *a, *b, *c;
    struct tcm_region *r1, *r2;
    struct tcm_state *c1, *c2, *c3;
    struct tcm_state *c31, *c32, *c33;

    rc = tcm_model_create(&model, "Test");
    ASSERT_EQ(rc, TCM_OK);
    ASSERT(model != NULL);
    ASSERT_EQ((char *) tcm_model_name(model), "Test");

    rc = tcm_set_region_name(model->root, "Root region");
    ASSERT_EQ(rc, TCM_OK);

    rc = tcm_add_state(model->root, "A", &a);
    ASSERT_EQ(rc, TCM_OK);

    rc = tcm_add_state(model->root, "B", &b);
    ASSERT_EQ(rc, TCM_OK);

    rc = tcm_add_state(model->root, "C", &c);
    ASSERT_EQ(rc, TCM_OK);

    /* Add region in composit state C */
    rc = tcm_add_region(c, false, &r1);
    ASSERT_EQ(rc, TCM_OK);

    rc = tcm_set_region_name(r1, "Another region");
    ASSERT_EQ(rc, TCM_OK);

    /* Add some new states in r1 */

    rc = tcm_add_state(r1, "C1", &c1);
    ASSERT_EQ(rc, TCM_OK);

    rc = tcm_add_state(r1, "C2", &c2);
    ASSERT_EQ(rc, TCM_OK);

    rc = tcm_add_state(r1, "C3", &c3);
    ASSERT_EQ(rc, TCM_OK);

    /* Create region in composit state C3 */

    rc = tcm_add_region(c3, false, &r2);
    ASSERT_EQ(rc, TCM_OK);

    rc = tcm_set_region_name(r2, "Yet another region");
    ASSERT_EQ(rc, TCM_OK);

    /* Add some new states in r2 */

    rc = tcm_add_state(r2, "C31", &c31);
    ASSERT_EQ(rc, TCM_OK);

    rc = tcm_add_state(r2, "C32", &c32);
    ASSERT_EQ(rc, TCM_OK);

    rc = tcm_add_state(r2, "C33", &c33);
    ASSERT_EQ(rc, TCM_OK);

    rc = tcm_model_write("test_model_out.tcm", model);
    ASSERT_EQ(rc, TCM_OK);

    rc = tcm_model_free(model);
    ASSERT_EQ(rc, TCM_OK);
}

TEST(load_model3)
{
    int rc;
    struct tcm_model *model;

    rc = tcm_model_load("test_model_out.tcm", &model);
    ASSERT_EQ(rc, TCM_OK);

    printf("Model load %i\n", rc);

    json_object_object_foreach(model->jroot, key, val)
    {
        printf("%s: %s\n", key, json_object_get_string(val));
    }


    rc = tcm_model_free(model);
    ASSERT_EQ(rc, TCM_OK);
}
