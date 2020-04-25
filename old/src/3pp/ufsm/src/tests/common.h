#ifndef UFSM_TEST_COMMON
#define UFSM_TEST_COMMON

#include <assert.h>
#include <ufsm.h>

void test_process(struct ufsm_machine *m, uint32_t ev);
void test_init(struct ufsm_machine *m);

#endif
