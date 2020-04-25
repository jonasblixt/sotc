#include <stdio.h>
#include <assert.h>
#include <ufsm.h>
#include <test_transition_prio_input.h>
#include "common.h"

static bool flag_eA,flag_xA,flag_eB,flag_xB,flag_eC,flag_xC,flag_eD,flag_xD,
            flag_eE, flag_eF, flag_xF, flag_eG, flag_xG, flag_eH, flag_xH;

static void reset_flags(void)
{
    flag_eA = false;
    flag_xA = false;
    flag_eB = false;
    flag_xB = false;
    flag_eC = false;
    flag_xC = false;
    flag_eD = false;
    flag_xD = false;
    flag_eE = false;
    flag_eF = false;
    flag_xF = false;
    flag_eG = false;
    flag_xG = false;
    flag_eH = false;
    flag_xH = false;
}

void eA(void)
{
    flag_eA = true;
}

void xA(void)
{
    flag_xA = true;

    assert ("xC" && flag_xD && flag_xC && flag_xH && flag_xB
                 && flag_xA && flag_xF);
}

void eB(void)
{
    flag_eB = true;
}

void xB(void)
{
    flag_xB = true;

    assert ("xB" && flag_xD && !flag_xC && flag_xH && flag_xB && !flag_xC
                 && !flag_xA && flag_xF);
}

void eC(void)
{
    flag_eC = true;
}

void xC(void)
{
    flag_xC = true;

    assert ("xC" && flag_xD && flag_xC && flag_xH && flag_xB
                 && !flag_xA && flag_xF);
}

void eD(void)
{
    flag_eD = true;
}

void xD(void)
{
    flag_xD = true;

    assert ("xD" && flag_xD && !flag_xC && !flag_xH && !flag_xB && !flag_xC
                 && !flag_xA && flag_xF);
}

void eE(void)
{
    flag_eE = true;
}

void eF(void)
{
    flag_eF = true;
    assert ("eF" && !flag_eH);
}

void xF(void)
{
    flag_xF = true;

    assert ("xF" && !flag_xD && !flag_xC && !flag_xH && !flag_xB && !flag_xC
                 && !flag_xA);
}

void eG(void)
{
    flag_eG = true;
}

void xG(void)
{
    flag_xG = true;
}

void eH(void)
{
    flag_eH = true;

    assert ("eH" && flag_eF);
}

void xH(void)
{
    flag_xH = true;

    assert ("xH" && flag_xD && !flag_xC && flag_xH && !flag_xB && !flag_xC
                 && !flag_xA && flag_xF);

}

int main(void) 
{
    struct ufsm_machine *m = get_StateMachine1();
    
    test_init(m);
    ufsm_init_machine(m);

    assert ("step1" && flag_eA && flag_eC && flag_eD && flag_eE && flag_eB
                    && flag_eG && !flag_xA && !flag_xD && !flag_eF && !flag_xB
                    && !flag_xG && !flag_eH && !flag_xH);
                    
    test_process(m, EV);

    reset_flags();

    test_process(m, EV2);

    return 0;
}
