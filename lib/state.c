#include <stdio.h>
#include <string.h>
#include <tcm/tcm.h>
#include <tcm/model.h>
#include <tcm/state.h>
#include <json.h>


int tcm_state_add(struct tcm_region *region, const char *name)
{
    struct tcm_state *state = malloc(sizeof(struct tcm_state));

    if (!state)
        return -TCM_ERR_MEM;

    memset(state, 0, sizeof(*state));


    state->name = strdup(name);

    if (region->last_state)
        region->last_state->next = state;
    if (!region->state)
        region->state = state;

    region->last_state = state;

    return TCM_OK;
}
