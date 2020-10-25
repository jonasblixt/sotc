#include <string.h>
#include <sotc/sotc.h>
#include <sotc/model.h>
#include <json.h>
#include "nala.h"
#include "common.h"

TEST(create_entry_action)
{
    int rc;
    struct sotc_model *model;
    struct sotc_state *a, *b, *c;
    struct sotc_action *action, *action2;
    char *name;
    char uuid_str[37];
    char uuid_str2[37];

    printf("Creating model\n");
    rc = sotc_model_create(&model, "Test");
    ASSERT_EQ(rc, SOTC_OK);
    ASSERT(model != NULL);
    ASSERT_EQ((char *) sotc_model_name(model), "Test");

    rc = sotc_set_region_name(model->root, "Root region");
    ASSERT_EQ(rc, SOTC_OK);

    printf("Adding state\n");
    rc = sotc_add_state(model->root, "A", &a);
    ASSERT_EQ(rc, SOTC_OK);

    rc = sotc_model_add_action(model, SOTC_ACTION_ENTRY,
                                     "test-action", &action);
    ASSERT_EQ(rc, SOTC_OK);

    uuid_unparse(action->id, uuid_str);
    printf("Added action '%s'\n", uuid_str);
    name = strdup(action->name);
    ASSERT_EQ(name, "test-action");

    rc = sotc_model_write("test_entry_action.sotc", model);
    ASSERT_EQ(rc, SOTC_OK);

    rc = sotc_model_free(model);
    ASSERT_EQ(rc, SOTC_OK);

    /* Re-load model */

    rc = sotc_model_load("test_entry_action.sotc", &model);
    ASSERT_EQ(rc, SOTC_OK);

    printf("Model load %i\n", rc);
    printf("model->actions = %p\n", model->actions);

    uuid_unparse(model->entries->id, uuid_str2);
    ASSERT_EQ((char *) uuid_str, (char *) uuid_str2);
    ASSERT_EQ(model->entries->name, "test-action");

    rc = sotc_model_free(model);
    ASSERT_EQ(rc, SOTC_OK);
    free(name);
}
