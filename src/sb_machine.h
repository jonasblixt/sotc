#ifndef UFSM_GEN_sb_machine_H__
#define UFSM_GEN_sb_machine_H__
#include <ufsm.h>
#ifndef NULL
 #define NULL (void *) 0
#endif
void resize_component(void);
void select_component(void);
void save_offset(void);
void move_component(void);
void show_add_menu(void);
void hide_add_menu(void);
void stop_add_menu_timer(void);
void start_add_menu_timer(void);
void finalize_empty_state(void);
void update_empty_state2(void);
void update_empty_state1(void);
void create_empty_state(void);
enum {
  EV_KEY_A,
  EV_COMPLETED,
  EV_LEFT_CLICK,
  EV_LEFT_CLICK_UP,
  EV_SELECTED,
  EV_CURSOR,
  EV_MOVE_VERTICE,
  EV_ADD_MENU_TIMER,
  EV_KEY_S,
  EV_ADD_STATE,
  EV_ENTER,
};
struct ufsm_machine * get_MainMachine(void);
struct ufsm_machine * get_AddMachine(void);
struct ufsm_machine * get_AddState(void);
#endif
