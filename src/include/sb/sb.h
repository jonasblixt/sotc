#ifndef __SB_H__
#define __SB_H__

#include <stdint.h>
#include <stdbool.h>
#include <nanovg.h>

enum 
{
  SB_OK,
  SB_ERR
};

typedef void  (*sb_key_event_t) (struct sb_context *context,
                                 int key_code,
                                 int key_action,
                                 int key_modifier);

struct sb_context
{
  int window_size_x;
  int window_size_y;
  sb_key_event_t key_event;
  bool ready;
};

uint32_t sb_initialize(struct sb_context *context);


/* Operating System Abstraction Layer */
uint32_t osal_get_window_size(struct sb_context *context,
                              unsigned int *size_x,
                              unsigned int *size_y);

uint32_t osal_get_us_counter(struct sb_context *context,
                             uint64_t *counter);

uint32_t osal_get_mouse_position(struct sb_context *context,
                                 unsigned int *pos_x,
                                 unsigned int *pos_y);

uint32_t osal_exit(struct sb_context *context);

#endif
