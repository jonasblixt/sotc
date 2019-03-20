#include "sb_machine.h"
static struct ufsm_machine AAAAAAFpiu9KcUPSZRw;
static struct ufsm_region AAAAAAFpiu9KckPTNd0;
static struct ufsm_transition AAAAAAFpivcqb0QSc8c;
static struct ufsm_transition AAAAAAFpiwAn7kRR__w;
static struct ufsm_transition AAAAAAFpiwCdYURrXh0;
static struct ufsm_transition AAAAAAFpjRhC55O0Bvc;
static struct ufsm_transition AAAAAAFpjRiR3JPKQYg;
static struct ufsm_state AAAAAAFpjXdDaZRjNQo;
static struct ufsm_region AAAAAAFpjXeoAZSIsb4;
static struct ufsm_transition AAAAAAFpjXofRZS7qrU;
static struct ufsm_transition AAAAAAFpjYnst5UrQO8;
static struct ufsm_transition AAAAAAFpjYqbT5VbPyI;
static struct ufsm_transition AAAAAAFpkfw_S5YVVQQ;
static struct ufsm_transition AAAAAAFpkfzgPpYtu1U;
static struct ufsm_state AAAAAAFpkfil8ZWXpHg;
static struct ufsm_entry_exit AAAAAAFpkf1LBZZG7nU;
static struct ufsm_state AAAAAAFpjRgoFpOOfN8;
static struct ufsm_entry_exit AAAAAAFpjRi7yJPdn8U;
static struct ufsm_entry_exit AAAAAAFpkfvhjpXrqjY;
static struct ufsm_state AAAAAAFpjVzCKpP19Ls;
static struct ufsm_entry_exit AAAAAAFpjV38hJRMDEg;
static struct ufsm_state AAAAAAFpjXoe3ZSq8mQ;
static struct ufsm_state AAAAAAFpiwAKFEQrE2s;
static struct ufsm_state AAAAAAFpiu_hkEPZeYQ;
static struct ufsm_state AAAAAAFpivcqJkQB6eo;
static struct ufsm_machine AAAAAAFpiwD0KER_f78;
static struct ufsm_region AAAAAAFpiwD0KUR_drY;
static struct ufsm_transition AAAAAAFpiwLXs0UZTAo;
static struct ufsm_state AAAAAAFpiwHSakSK6Es;
static struct ufsm_region AAAAAAFpiwJYxkTbGmk;
static struct ufsm_transition AAAAAAFpiwKLskT2iCg;
static struct ufsm_transition AAAAAAFpiwSONEVuijg;
static struct ufsm_transition AAAAAAFpiwWX8EWRkiU;
static struct ufsm_transition AAAAAAFpixC_0EXaK2U;
static struct ufsm_state AAAAAAFpixBgOkWwapM;
static struct ufsm_state AAAAAAFpiwREYUVF13U;
static struct ufsm_entry_exit AAAAAAFpiwToVkWH26g;
static struct ufsm_state AAAAAAFpiwIphkSynKw;
static struct ufsm_entry_exit AAAAAAFpix9kGkgTlgw;
static struct ufsm_entry_exit AAAAAAFpiwZ2b0WkyI4;
static struct ufsm_state AAAAAAFpiwKLW0TlsYg;
static struct ufsm_entry_exit AAAAAAFpiwL2ukUrRGM;
static struct ufsm_state AAAAAAFpiwLXYEUIU3c;
static struct ufsm_machine AAAAAAFpixPETEYRenk;
static struct ufsm_region AAAAAAFpixPETEYSvNg;
static struct ufsm_transition AAAAAAFpjCyK0ppd8z4;
static struct ufsm_state AAAAAAFpjCqk0Jnpm_U;
static struct ufsm_region AAAAAAFpjCrT_JoP12o;
static struct ufsm_transition AAAAAAFpixaNAEbHB7U;
static struct ufsm_transition AAAAAAFpixgJvEcR2Gw;
static struct ufsm_transition AAAAAAFpixihvUcxF6U;
static struct ufsm_transition AAAAAAFpixmWT0eMxfg;
static struct ufsm_transition AAAAAAFpixoPJkf3acc;
static struct ufsm_transition AAAAAAFpjBM3G5mGEH4;
static struct ufsm_state AAAAAAFpixnzukfRLYk;
static struct ufsm_entry_exit AAAAAAFpi1E1yUgeyoY;
static struct ufsm_state AAAAAAFpixe5OUbnaEs;
static struct ufsm_entry_exit AAAAAAFpixhLQkcoDTs;
static struct ufsm_state AAAAAAFpixWWP0ZvutE;
static struct ufsm_entry_exit AAAAAAFpixbJlkbfkfs;
static struct ufsm_state AAAAAAFpjBMBtJlduXM;
static struct ufsm_entry_exit AAAAAAFpjBNHq5mX0CU;
static struct ufsm_state AAAAAAFpixmV90d7vtY;
static struct ufsm_state AAAAAAFpjCyKeZpMuKE;



static struct ufsm_machine AAAAAAFpiu9KcUPSZRw = {
  .id     = "AAAAAAFpiu9KcUPSZRw=", 
  .name   = "MainMachine", 
  .region = &AAAAAAFpiu9KckPTNd0,    
  .next = &AAAAAAFpiwD0KER_f78, 
  .parent_state = NULL, 
};
static struct ufsm_region AAAAAAFpiu9KckPTNd0 = {
  .id = "AAAAAAFpiu9KckPTNd0=",
  .name = "MainMachineregion0",
  .state = &AAAAAAFpjXdDaZRjNQo,
  .has_history = false,
  .history = NULL,
  .transition = &AAAAAAFpivcqb0QSc8c,
  .parent_state = NULL,
  .next = NULL,
};
static struct ufsm_transition AAAAAAFpivcqb0QSc8c = {
  .id = "AAAAAAFpivcqb0QSc8c=",
  .name = "",
  .trigger = NULL,
  .kind = 0,
  .action = NULL,
  .defer = false,
  .guard = NULL,
  .source = &AAAAAAFpivcqJkQB6eo,
  .dest = &AAAAAAFpiu_hkEPZeYQ,
  .next = &AAAAAAFpiwAn7kRR__w,
};
static struct ufsm_trigger AAAAAAFpiwAn7kRR__w_triggers[] = {
{
  .trigger = 0,
  .name = "EV_KEY_A",
},
 {
  .trigger = -1,
  .name = NULL,
},
};
static struct ufsm_transition AAAAAAFpiwAn7kRR__w = {
  .id = "AAAAAAFpiwAn7kRR//w=",
  .name = "",
  .trigger = AAAAAAFpiwAn7kRR__w_triggers,
  .kind = 0,
  .action = NULL,
  .defer = false,
  .guard = NULL,
  .source = &AAAAAAFpiu_hkEPZeYQ,
  .dest = &AAAAAAFpiwAKFEQrE2s,
  .next = &AAAAAAFpiwCdYURrXh0,
};
static struct ufsm_trigger AAAAAAFpiwCdYURrXh0_triggers[] = {
{
  .trigger = 1,
  .name = "EV_COMPLETED",
},
 {
  .trigger = -1,
  .name = NULL,
},
};
static struct ufsm_transition AAAAAAFpiwCdYURrXh0 = {
  .id = "AAAAAAFpiwCdYURrXh0=",
  .name = "",
  .trigger = AAAAAAFpiwCdYURrXh0_triggers,
  .kind = 0,
  .action = NULL,
  .defer = false,
  .guard = NULL,
  .source = &AAAAAAFpiwAKFEQrE2s,
  .dest = &AAAAAAFpiu_hkEPZeYQ,
  .next = &AAAAAAFpjRhC55O0Bvc,
};
static struct ufsm_trigger AAAAAAFpjRhC55O0Bvc_triggers[] = {
{
  .trigger = 2,
  .name = "EV_LEFT_CLICK",
},
 {
  .trigger = -1,
  .name = NULL,
},
};
static struct ufsm_transition AAAAAAFpjRhC55O0Bvc = {
  .id = "AAAAAAFpjRhC55O0Bvc=",
  .name = "",
  .trigger = AAAAAAFpjRhC55O0Bvc_triggers,
  .kind = 0,
  .action = NULL,
  .defer = false,
  .guard = NULL,
  .source = &AAAAAAFpiu_hkEPZeYQ,
  .dest = &AAAAAAFpjXdDaZRjNQo,
  .next = &AAAAAAFpjRiR3JPKQYg,
};
static struct ufsm_trigger AAAAAAFpjRiR3JPKQYg_triggers[] = {
{
  .trigger = 3,
  .name = "EV_LEFT_CLICK_UP",
},
 {
  .trigger = -1,
  .name = NULL,
},
};
static struct ufsm_transition AAAAAAFpjRiR3JPKQYg = {
  .id = "AAAAAAFpjRiR3JPKQYg=",
  .name = "",
  .trigger = AAAAAAFpjRiR3JPKQYg_triggers,
  .kind = 0,
  .action = NULL,
  .defer = false,
  .guard = NULL,
  .source = &AAAAAAFpjXdDaZRjNQo,
  .dest = &AAAAAAFpiu_hkEPZeYQ,
  .next = NULL,
};
static struct ufsm_state AAAAAAFpjXdDaZRjNQo = {
  .id   = "AAAAAAFpjXdDaZRjNQo=",
  .name = "SelectMove",
  .kind = 0,
  .parent_region = &AAAAAAFpiu9KckPTNd0,
  .entry = NULL,
  .doact = NULL,
  .exit = NULL,
  .region = &AAAAAAFpjXeoAZSIsb4,
  .submachine = NULL,
  .next = &AAAAAAFpiwAKFEQrE2s,
};
static struct ufsm_region AAAAAAFpjXeoAZSIsb4 = {
  .id = "AAAAAAFpjXeoAZSIsb4=",
  .name = "Region1",
  .state = &AAAAAAFpkfil8ZWXpHg,
  .has_history = false,
  .history = NULL,
  .transition = &AAAAAAFpjXofRZS7qrU,
  .parent_state = &AAAAAAFpjXdDaZRjNQo,
  .next = NULL,
};
static struct ufsm_transition AAAAAAFpjXofRZS7qrU = {
  .id = "AAAAAAFpjXofRZS7qrU=",
  .name = "",
  .trigger = NULL,
  .kind = 0,
  .action = NULL,
  .defer = false,
  .guard = NULL,
  .source = &AAAAAAFpjXoe3ZSq8mQ,
  .dest = &AAAAAAFpjRgoFpOOfN8,
  .next = &AAAAAAFpjYnst5UrQO8,
};
static struct ufsm_trigger AAAAAAFpjYnst5UrQO8_triggers[] = {
{
  .trigger = 4,
  .name = "EV_SELECTED",
},
 {
  .trigger = -1,
  .name = NULL,
},
};
static struct ufsm_transition AAAAAAFpjYnst5UrQO8 = {
  .id = "AAAAAAFpjYnst5UrQO8=",
  .name = "",
  .trigger = AAAAAAFpjYnst5UrQO8_triggers,
  .kind = 0,
  .action = NULL,
  .defer = false,
  .guard = NULL,
  .source = &AAAAAAFpjRgoFpOOfN8,
  .dest = &AAAAAAFpjVzCKpP19Ls,
  .next = &AAAAAAFpjYqbT5VbPyI,
};
static struct ufsm_trigger AAAAAAFpjYqbT5VbPyI_triggers[] = {
{
  .trigger = 5,
  .name = "EV_CURSOR",
},
 {
  .trigger = -1,
  .name = NULL,
},
};
static struct ufsm_transition AAAAAAFpjYqbT5VbPyI = {
  .id = "AAAAAAFpjYqbT5VbPyI=",
  .name = "",
  .trigger = AAAAAAFpjYqbT5VbPyI_triggers,
  .kind = 0,
  .action = NULL,
  .defer = false,
  .guard = NULL,
  .source = &AAAAAAFpjVzCKpP19Ls,
  .dest = &AAAAAAFpjVzCKpP19Ls,
  .next = &AAAAAAFpkfw_S5YVVQQ,
};
static struct ufsm_trigger AAAAAAFpkfw_S5YVVQQ_triggers[] = {
{
  .trigger = 6,
  .name = "EV_MOVE_VERTICE",
},
 {
  .trigger = -1,
  .name = NULL,
},
};
static struct ufsm_transition AAAAAAFpkfw_S5YVVQQ = {
  .id = "AAAAAAFpkfw+S5YVVQQ=",
  .name = "",
  .trigger = AAAAAAFpkfw_S5YVVQQ_triggers,
  .kind = 0,
  .action = NULL,
  .defer = false,
  .guard = NULL,
  .source = &AAAAAAFpjRgoFpOOfN8,
  .dest = &AAAAAAFpkfil8ZWXpHg,
  .next = &AAAAAAFpkfzgPpYtu1U,
};
static struct ufsm_trigger AAAAAAFpkfzgPpYtu1U_triggers[] = {
{
  .trigger = 5,
  .name = "EV_CURSOR",
},
 {
  .trigger = -1,
  .name = NULL,
},
};
static struct ufsm_transition AAAAAAFpkfzgPpYtu1U = {
  .id = "AAAAAAFpkfzgPpYtu1U=",
  .name = "",
  .trigger = AAAAAAFpkfzgPpYtu1U_triggers,
  .kind = 0,
  .action = NULL,
  .defer = false,
  .guard = NULL,
  .source = &AAAAAAFpkfil8ZWXpHg,
  .dest = &AAAAAAFpkfil8ZWXpHg,
  .next = NULL,
};
static struct ufsm_state AAAAAAFpkfil8ZWXpHg = {
  .id   = "AAAAAAFpkfil8ZWXpHg=",
  .name = "Resize",
  .kind = 0,
  .parent_region = &AAAAAAFpjXeoAZSIsb4,
  .entry = &AAAAAAFpkf1LBZZG7nU,
  .doact = NULL,
  .exit = NULL,
  .region = NULL,
  .submachine = NULL,
  .next = &AAAAAAFpjRgoFpOOfN8,
};
static struct ufsm_entry_exit AAAAAAFpkf1LBZZG7nU = {
  .id = "AAAAAAFpkf1LBZZG7nU=",
  .name = "resize_component",
  .f = &resize_component,
  .next = NULL,
};
static struct ufsm_state AAAAAAFpjRgoFpOOfN8 = {
  .id   = "AAAAAAFpjRgoFpOOfN8=",
  .name = "Select",
  .kind = 0,
  .parent_region = &AAAAAAFpjXeoAZSIsb4,
  .entry = &AAAAAAFpjRi7yJPdn8U,
  .doact = NULL,
  .exit = &AAAAAAFpkfvhjpXrqjY,
  .region = NULL,
  .submachine = NULL,
  .next = &AAAAAAFpjVzCKpP19Ls,
};
static struct ufsm_entry_exit AAAAAAFpjRi7yJPdn8U = {
  .id = "AAAAAAFpjRi7yJPdn8U=",
  .name = "select_component",
  .f = &select_component,
  .next = NULL,
};
static struct ufsm_entry_exit AAAAAAFpkfvhjpXrqjY = {
  .id = "AAAAAAFpkfvhjpXrqjY=",
  .name = "save_offset",
  .f = &save_offset,
  .next = NULL,
};
static struct ufsm_state AAAAAAFpjVzCKpP19Ls = {
  .id   = "AAAAAAFpjVzCKpP19Ls=",
  .name = "Move",
  .kind = 0,
  .parent_region = &AAAAAAFpjXeoAZSIsb4,
  .entry = &AAAAAAFpjV38hJRMDEg,
  .doact = NULL,
  .exit = NULL,
  .region = NULL,
  .submachine = NULL,
  .next = &AAAAAAFpjXoe3ZSq8mQ,
};
static struct ufsm_entry_exit AAAAAAFpjV38hJRMDEg = {
  .id = "AAAAAAFpjV38hJRMDEg=",
  .name = "move_component",
  .f = &move_component,
  .next = NULL,
};
static struct ufsm_state AAAAAAFpjXoe3ZSq8mQ = {
  .id   = "AAAAAAFpjXoe3ZSq8mQ=",
  .name = "Init",
  .kind = 1,
  .parent_region = &AAAAAAFpjXeoAZSIsb4,
  .entry = NULL,
  .doact = NULL,
  .exit = NULL,
  .region = NULL,
  .submachine = NULL,
  .next = NULL,
};
static struct ufsm_state AAAAAAFpiwAKFEQrE2s = {
  .id   = "AAAAAAFpiwAKFEQrE2s=",
  .name = "Add",
  .kind = 0,
  .parent_region = &AAAAAAFpiu9KckPTNd0,
  .entry = NULL,
  .doact = NULL,
  .exit = NULL,
  .region = &AAAAAAFpiwD0KUR_drY,
  .submachine = NULL,
  .next = &AAAAAAFpiu_hkEPZeYQ,
};
static struct ufsm_state AAAAAAFpiu_hkEPZeYQ = {
  .id   = "AAAAAAFpiu/hkEPZeYQ=",
  .name = "Idle",
  .kind = 0,
  .parent_region = &AAAAAAFpiu9KckPTNd0,
  .entry = NULL,
  .doact = NULL,
  .exit = NULL,
  .region = NULL,
  .submachine = NULL,
  .next = &AAAAAAFpivcqJkQB6eo,
};
static struct ufsm_state AAAAAAFpivcqJkQB6eo = {
  .id   = "AAAAAAFpivcqJkQB6eo=",
  .name = "Init",
  .kind = 1,
  .parent_region = &AAAAAAFpiu9KckPTNd0,
  .entry = NULL,
  .doact = NULL,
  .exit = NULL,
  .region = NULL,
  .submachine = NULL,
  .next = NULL,
};
static struct ufsm_machine AAAAAAFpiwD0KER_f78 = {
  .id     = "AAAAAAFpiwD0KER+f78=", 
  .name   = "AddMachine", 
  .region = &AAAAAAFpiwD0KUR_drY,    
  .next = &AAAAAAFpixPETEYRenk, 
  .parent_state = NULL, 
};
static struct ufsm_region AAAAAAFpiwD0KUR_drY = {
  .id = "AAAAAAFpiwD0KUR/drY=",
  .name = "AddMachineregion0",
  .state = &AAAAAAFpiwHSakSK6Es,
  .has_history = false,
  .history = NULL,
  .transition = &AAAAAAFpiwLXs0UZTAo,
  .parent_state = &AAAAAAFpiwAKFEQrE2s,
  .next = NULL,
};
static struct ufsm_transition AAAAAAFpiwLXs0UZTAo = {
  .id = "AAAAAAFpiwLXs0UZTAo=",
  .name = "",
  .trigger = NULL,
  .kind = 0,
  .action = NULL,
  .defer = false,
  .guard = NULL,
  .source = &AAAAAAFpiwLXYEUIU3c,
  .dest = &AAAAAAFpiwHSakSK6Es,
  .next = NULL,
};
static struct ufsm_state AAAAAAFpiwHSakSK6Es = {
  .id   = "AAAAAAFpiwHSakSK6Es=",
  .name = "Add",
  .kind = 0,
  .parent_region = &AAAAAAFpiwD0KUR_drY,
  .entry = &AAAAAAFpiwL2ukUrRGM,
  .doact = NULL,
  .exit = NULL,
  .region = &AAAAAAFpiwJYxkTbGmk,
  .submachine = NULL,
  .next = &AAAAAAFpiwLXYEUIU3c,
};
static struct ufsm_region AAAAAAFpiwJYxkTbGmk = {
  .id = "AAAAAAFpiwJYxkTbGmk=",
  .name = "Region1",
  .state = &AAAAAAFpixBgOkWwapM,
  .has_history = false,
  .history = NULL,
  .transition = &AAAAAAFpiwKLskT2iCg,
  .parent_state = &AAAAAAFpiwHSakSK6Es,
  .next = NULL,
};
static struct ufsm_transition AAAAAAFpiwKLskT2iCg = {
  .id = "AAAAAAFpiwKLskT2iCg=",
  .name = "",
  .trigger = NULL,
  .kind = 0,
  .action = NULL,
  .defer = false,
  .guard = NULL,
  .source = &AAAAAAFpiwKLW0TlsYg,
  .dest = &AAAAAAFpiwIphkSynKw,
  .next = &AAAAAAFpiwSONEVuijg,
};
static struct ufsm_trigger AAAAAAFpiwSONEVuijg_triggers[] = {
{
  .trigger = 7,
  .name = "EV_ADD_MENU_TIMER",
},
 {
  .trigger = -1,
  .name = NULL,
},
};
static struct ufsm_transition AAAAAAFpiwSONEVuijg = {
  .id = "AAAAAAFpiwSONEVuijg=",
  .name = "",
  .trigger = AAAAAAFpiwSONEVuijg_triggers,
  .kind = 0,
  .action = NULL,
  .defer = false,
  .guard = NULL,
  .source = &AAAAAAFpiwIphkSynKw,
  .dest = &AAAAAAFpiwREYUVF13U,
  .next = &AAAAAAFpiwWX8EWRkiU,
};
static struct ufsm_transition AAAAAAFpiwWX8EWRkiU = {
  .id = "AAAAAAFpiwWX8EWRkiU=",
  .name = "",
  .trigger = NULL,
  .kind = 0,
  .action = NULL,
  .defer = false,
  .guard = NULL,
  .source = &AAAAAAFpiwREYUVF13U,
  .dest = &AAAAAAFpiwIphkSynKw,
  .next = &AAAAAAFpixC_0EXaK2U,
};
static struct ufsm_trigger AAAAAAFpixC_0EXaK2U_triggers[] = {
{
  .trigger = 8,
  .name = "EV_KEY_S",
},
{
  .trigger = 9,
  .name = "EV_ADD_STATE",
},
 {
  .trigger = -1,
  .name = NULL,
},
};
static struct ufsm_transition AAAAAAFpixC_0EXaK2U = {
  .id = "AAAAAAFpixC/0EXaK2U=",
  .name = "",
  .trigger = AAAAAAFpixC_0EXaK2U_triggers,
  .kind = 0,
  .action = NULL,
  .defer = false,
  .guard = NULL,
  .source = &AAAAAAFpiwIphkSynKw,
  .dest = &AAAAAAFpixBgOkWwapM,
  .next = NULL,
};
static struct ufsm_state AAAAAAFpixBgOkWwapM = {
  .id   = "AAAAAAFpixBgOkWwapM=",
  .name = "AddState",
  .kind = 0,
  .parent_region = &AAAAAAFpiwJYxkTbGmk,
  .entry = NULL,
  .doact = NULL,
  .exit = NULL,
  .region = &AAAAAAFpixPETEYSvNg,
  .submachine = NULL,
  .next = &AAAAAAFpiwREYUVF13U,
};
static struct ufsm_state AAAAAAFpiwREYUVF13U = {
  .id   = "AAAAAAFpiwREYUVF13U=",
  .name = "EnableAddMenu",
  .kind = 0,
  .parent_region = &AAAAAAFpiwJYxkTbGmk,
  .entry = &AAAAAAFpiwToVkWH26g,
  .doact = NULL,
  .exit = NULL,
  .region = NULL,
  .submachine = NULL,
  .next = &AAAAAAFpiwIphkSynKw,
};
static struct ufsm_entry_exit AAAAAAFpiwToVkWH26g = {
  .id = "AAAAAAFpiwToVkWH26g=",
  .name = "show_add_menu",
  .f = &show_add_menu,
  .next = NULL,
};
static struct ufsm_state AAAAAAFpiwIphkSynKw = {
  .id   = "AAAAAAFpiwIphkSynKw=",
  .name = "Wait",
  .kind = 0,
  .parent_region = &AAAAAAFpiwJYxkTbGmk,
  .entry = NULL,
  .doact = NULL,
  .exit = &AAAAAAFpix9kGkgTlgw,
  .region = NULL,
  .submachine = NULL,
  .next = &AAAAAAFpiwKLW0TlsYg,
};
static struct ufsm_entry_exit AAAAAAFpix9kGkgTlgw = {
  .id = "AAAAAAFpix9kGkgTlgw=",
  .name = "hide_add_menu",
  .f = &hide_add_menu,
  .next = &AAAAAAFpiwZ2b0WkyI4,
};
static struct ufsm_entry_exit AAAAAAFpiwZ2b0WkyI4 = {
  .id = "AAAAAAFpiwZ2b0WkyI4=",
  .name = "stop_add_menu_timer",
  .f = &stop_add_menu_timer,
  .next = NULL,
};
static struct ufsm_state AAAAAAFpiwKLW0TlsYg = {
  .id   = "AAAAAAFpiwKLW0TlsYg=",
  .name = "Init",
  .kind = 1,
  .parent_region = &AAAAAAFpiwJYxkTbGmk,
  .entry = NULL,
  .doact = NULL,
  .exit = NULL,
  .region = NULL,
  .submachine = NULL,
  .next = NULL,
};
static struct ufsm_entry_exit AAAAAAFpiwL2ukUrRGM = {
  .id = "AAAAAAFpiwL2ukUrRGM=",
  .name = "start_add_menu_timer",
  .f = &start_add_menu_timer,
  .next = NULL,
};
static struct ufsm_state AAAAAAFpiwLXYEUIU3c = {
  .id   = "AAAAAAFpiwLXYEUIU3c=",
  .name = "Init",
  .kind = 1,
  .parent_region = &AAAAAAFpiwD0KUR_drY,
  .entry = NULL,
  .doact = NULL,
  .exit = NULL,
  .region = NULL,
  .submachine = NULL,
  .next = NULL,
};
static struct ufsm_machine AAAAAAFpixPETEYRenk = {
  .id     = "AAAAAAFpixPETEYRenk=", 
  .name   = "AddState", 
  .region = &AAAAAAFpixPETEYSvNg,    
  .next = NULL,
  .parent_state = NULL, 
};
static struct ufsm_region AAAAAAFpixPETEYSvNg = {
  .id = "AAAAAAFpixPETEYSvNg=",
  .name = "AddStateregion0",
  .state = &AAAAAAFpjCqk0Jnpm_U,
  .has_history = false,
  .history = NULL,
  .transition = &AAAAAAFpjCyK0ppd8z4,
  .parent_state = &AAAAAAFpixBgOkWwapM,
  .next = NULL,
};
static struct ufsm_transition AAAAAAFpjCyK0ppd8z4 = {
  .id = "AAAAAAFpjCyK0ppd8z4=",
  .name = "",
  .trigger = NULL,
  .kind = 0,
  .action = NULL,
  .defer = false,
  .guard = NULL,
  .source = &AAAAAAFpjCyKeZpMuKE,
  .dest = &AAAAAAFpjCqk0Jnpm_U,
  .next = NULL,
};
static struct ufsm_state AAAAAAFpjCqk0Jnpm_U = {
  .id   = "AAAAAAFpjCqk0Jnpm+U=",
  .name = "State1",
  .kind = 0,
  .parent_region = &AAAAAAFpixPETEYSvNg,
  .entry = NULL,
  .doact = NULL,
  .exit = NULL,
  .region = &AAAAAAFpjCrT_JoP12o,
  .submachine = NULL,
  .next = &AAAAAAFpjCyKeZpMuKE,
};
static struct ufsm_region AAAAAAFpjCrT_JoP12o = {
  .id = "AAAAAAFpjCrT/JoP12o=",
  .name = "Region1",
  .state = &AAAAAAFpixnzukfRLYk,
  .has_history = false,
  .history = NULL,
  .transition = &AAAAAAFpixaNAEbHB7U,
  .parent_state = &AAAAAAFpjCqk0Jnpm_U,
  .next = NULL,
};
static struct ufsm_trigger AAAAAAFpixaNAEbHB7U_triggers[] = {
{
  .trigger = 5,
  .name = "EV_CURSOR",
},
 {
  .trigger = -1,
  .name = NULL,
},
};
static struct ufsm_transition AAAAAAFpixaNAEbHB7U = {
  .id = "AAAAAAFpixaNAEbHB7U=",
  .name = "",
  .trigger = AAAAAAFpixaNAEbHB7U_triggers,
  .kind = 0,
  .action = NULL,
  .defer = false,
  .guard = NULL,
  .source = &AAAAAAFpixWWP0ZvutE,
  .dest = &AAAAAAFpixWWP0ZvutE,
  .next = &AAAAAAFpixgJvEcR2Gw,
};
static struct ufsm_trigger AAAAAAFpixgJvEcR2Gw_triggers[] = {
{
  .trigger = 10,
  .name = "EV_ENTER",
},
{
  .trigger = 2,
  .name = "EV_LEFT_CLICK",
},
 {
  .trigger = -1,
  .name = NULL,
},
};
static struct ufsm_transition AAAAAAFpixgJvEcR2Gw = {
  .id = "AAAAAAFpixgJvEcR2Gw=",
  .name = "",
  .trigger = AAAAAAFpixgJvEcR2Gw_triggers,
  .kind = 0,
  .action = NULL,
  .defer = false,
  .guard = NULL,
  .source = &AAAAAAFpixWWP0ZvutE,
  .dest = &AAAAAAFpixe5OUbnaEs,
  .next = &AAAAAAFpixihvUcxF6U,
};
static struct ufsm_trigger AAAAAAFpixihvUcxF6U_triggers[] = {
{
  .trigger = 5,
  .name = "EV_CURSOR",
},
 {
  .trigger = -1,
  .name = NULL,
},
};
static struct ufsm_transition AAAAAAFpixihvUcxF6U = {
  .id = "AAAAAAFpixihvUcxF6U=",
  .name = "",
  .trigger = AAAAAAFpixihvUcxF6U_triggers,
  .kind = 0,
  .action = NULL,
  .defer = false,
  .guard = NULL,
  .source = &AAAAAAFpixe5OUbnaEs,
  .dest = &AAAAAAFpixe5OUbnaEs,
  .next = &AAAAAAFpixmWT0eMxfg,
};
static struct ufsm_transition AAAAAAFpixmWT0eMxfg = {
  .id = "AAAAAAFpixmWT0eMxfg=",
  .name = "",
  .trigger = NULL,
  .kind = 0,
  .action = NULL,
  .defer = false,
  .guard = NULL,
  .source = &AAAAAAFpixmV90d7vtY,
  .dest = &AAAAAAFpjBMBtJlduXM,
  .next = &AAAAAAFpixoPJkf3acc,
};
static struct ufsm_trigger AAAAAAFpixoPJkf3acc_triggers[] = {
{
  .trigger = 10,
  .name = "EV_ENTER",
},
{
  .trigger = 2,
  .name = "EV_LEFT_CLICK",
},
 {
  .trigger = -1,
  .name = NULL,
},
};
static struct ufsm_transition AAAAAAFpixoPJkf3acc = {
  .id = "AAAAAAFpixoPJkf3acc=",
  .name = "",
  .trigger = AAAAAAFpixoPJkf3acc_triggers,
  .kind = 0,
  .action = NULL,
  .defer = false,
  .guard = NULL,
  .source = &AAAAAAFpixe5OUbnaEs,
  .dest = &AAAAAAFpixnzukfRLYk,
  .next = &AAAAAAFpjBM3G5mGEH4,
};
static struct ufsm_transition AAAAAAFpjBM3G5mGEH4 = {
  .id = "AAAAAAFpjBM3G5mGEH4=",
  .name = "",
  .trigger = NULL,
  .kind = 0,
  .action = NULL,
  .defer = false,
  .guard = NULL,
  .source = &AAAAAAFpjBMBtJlduXM,
  .dest = &AAAAAAFpixWWP0ZvutE,
  .next = NULL,
};
static struct ufsm_state AAAAAAFpixnzukfRLYk = {
  .id   = "AAAAAAFpixnzukfRLYk=",
  .name = "CreateState",
  .kind = 0,
  .parent_region = &AAAAAAFpjCrT_JoP12o,
  .entry = &AAAAAAFpi1E1yUgeyoY,
  .doact = NULL,
  .exit = NULL,
  .region = NULL,
  .submachine = NULL,
  .next = &AAAAAAFpixe5OUbnaEs,
};
static struct ufsm_entry_exit AAAAAAFpi1E1yUgeyoY = {
  .id = "AAAAAAFpi1E1yUgeyoY=",
  .name = "finalize_empty_state",
  .f = &finalize_empty_state,
  .next = NULL,
};
static struct ufsm_state AAAAAAFpixe5OUbnaEs = {
  .id   = "AAAAAAFpixe5OUbnaEs=",
  .name = "SelectSecondPoint",
  .kind = 0,
  .parent_region = &AAAAAAFpjCrT_JoP12o,
  .entry = &AAAAAAFpixhLQkcoDTs,
  .doact = NULL,
  .exit = NULL,
  .region = NULL,
  .submachine = NULL,
  .next = &AAAAAAFpixWWP0ZvutE,
};
static struct ufsm_entry_exit AAAAAAFpixhLQkcoDTs = {
  .id = "AAAAAAFpixhLQkcoDTs=",
  .name = "update_empty_state2",
  .f = &update_empty_state2,
  .next = NULL,
};
static struct ufsm_state AAAAAAFpixWWP0ZvutE = {
  .id   = "AAAAAAFpixWWP0ZvutE=",
  .name = "SelectFirstPoint",
  .kind = 0,
  .parent_region = &AAAAAAFpjCrT_JoP12o,
  .entry = &AAAAAAFpixbJlkbfkfs,
  .doact = NULL,
  .exit = NULL,
  .region = NULL,
  .submachine = NULL,
  .next = &AAAAAAFpjBMBtJlduXM,
};
static struct ufsm_entry_exit AAAAAAFpixbJlkbfkfs = {
  .id = "AAAAAAFpixbJlkbfkfs=",
  .name = "update_empty_state1",
  .f = &update_empty_state1,
  .next = NULL,
};
static struct ufsm_state AAAAAAFpjBMBtJlduXM = {
  .id   = "AAAAAAFpjBMBtJlduXM=",
  .name = "Init",
  .kind = 0,
  .parent_region = &AAAAAAFpjCrT_JoP12o,
  .entry = &AAAAAAFpjBNHq5mX0CU,
  .doact = NULL,
  .exit = NULL,
  .region = NULL,
  .submachine = NULL,
  .next = &AAAAAAFpixmV90d7vtY,
};
static struct ufsm_entry_exit AAAAAAFpjBNHq5mX0CU = {
  .id = "AAAAAAFpjBNHq5mX0CU=",
  .name = "create_empty_state",
  .f = &create_empty_state,
  .next = NULL,
};
static struct ufsm_state AAAAAAFpixmV90d7vtY = {
  .id   = "AAAAAAFpixmV90d7vtY=",
  .name = "Init",
  .kind = 1,
  .parent_region = &AAAAAAFpjCrT_JoP12o,
  .entry = NULL,
  .doact = NULL,
  .exit = NULL,
  .region = NULL,
  .submachine = NULL,
  .next = NULL,
};
static struct ufsm_state AAAAAAFpjCyKeZpMuKE = {
  .id   = "AAAAAAFpjCyKeZpMuKE=",
  .name = "Init",
  .kind = 1,
  .parent_region = &AAAAAAFpixPETEYSvNg,
  .entry = NULL,
  .doact = NULL,
  .exit = NULL,
  .region = NULL,
  .submachine = NULL,
  .next = NULL,
};

struct ufsm_machine * get_MainMachine(void) { return &AAAAAAFpiu9KcUPSZRw; }
struct ufsm_machine * get_AddMachine(void) { return &AAAAAAFpiwD0KER_f78; }
struct ufsm_machine * get_AddState(void) { return &AAAAAAFpixPETEYRenk; }
