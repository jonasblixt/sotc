#include <string.h>
#include <sotc/sotc.h>
#include <sotc/model.h>
#include <json.h>
#include "nala.h"
#include "common.h"

TEST(load_transitions)
{
    int rc;
    struct sotc_model *model;

    rc = sotc_model_load(SOTC_TEST_SRC"test_load_transitions.sotc", &model);
    ASSERT_EQ(rc, SOTC_OK);

    ASSERT(model->root->state);
    ASSERT(model->root->state->transition != NULL);

    rc = sotc_model_free(model);
    ASSERT_EQ(rc, SOTC_OK);
}
