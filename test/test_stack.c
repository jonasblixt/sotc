#include <string.h>
#include <tcm/tcm.h>
#include <tcm/stack.h>
#include "nala.h"
#include "common.h"

TEST(stack_create)
{
    int rc;
    struct tcm_stack *s = NULL;

    tcm_set_debug(tcm_test_debug);

    rc = tcm_stack_init(&s, 1024);
    ASSERT_EQ(rc, TCM_OK);

    rc = tcm_stack_free(s);
    ASSERT_EQ(rc, TCM_OK);
}


TEST(stack_test1)
{
    int rc;
    struct tcm_stack *s = NULL;

    tcm_set_debug(tcm_test_debug);

    rc = tcm_stack_init(&s, 1);
    ASSERT_EQ(rc, TCM_OK);

    int a = 1;

    rc = tcm_stack_push(s, &a);
    ASSERT_EQ(rc, TCM_OK);

    rc = tcm_stack_push(s, &a);
    ASSERT_EQ(rc, -TCM_ERROR);

    int *a_ptr = NULL;

    rc = tcm_stack_pop(s, (void *) &a_ptr);
    ASSERT_EQ(rc, TCM_OK);
    ASSERT(a_ptr == &a);

    rc = tcm_stack_pop(s, (void *) &a_ptr);
    ASSERT_EQ(rc, -TCM_ERROR);

    rc = tcm_stack_free(s);
    ASSERT_EQ(rc, TCM_OK);
}
