/**
 * uFSM
 *
 * Copyright (C) 2018 Jonas Persson <jonpe960@gmail.com>
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ufsm.h>

#include "output.h"

static FILE *fp_c = NULL;
static FILE *fp_h = NULL;

static uint32_t v = 0;
static bool flag_strip = false;

struct event_list
{
    char *name;
    uint32_t index;
    struct event_list *next;
};

static struct event_list *evlist;

static struct ufsm_entry_exit *eelist_first;
static struct ufsm_entry_exit **eelist = &eelist_first;
static struct ufsm_guard *guard_first;
static struct ufsm_guard **guard_list = &guard_first;
static struct ufsm_action *action_first;
static struct ufsm_action **action_list = &action_first;
static struct ufsm_doact *doact_first;
static struct ufsm_doact **doact_list = &doact_first;

static char * id_to_decl(const char *id)
{
    char * decl = malloc (strlen(id)+1);
    char * decl_ptr = decl;
    bzero(decl, strlen(id) + 1);

    do
    {
        *decl_ptr = *id++;
        if (*decl_ptr == '+')
            *decl_ptr = '_';
        if (*decl_ptr == '/')
            *decl_ptr = '_';
        if (*decl_ptr == '=')
            *decl_ptr = 0;

    } while (*decl_ptr++);

    return decl;
}

static uint32_t ev_name_to_index(const char *name)
{
    struct event_list *last_entry = evlist;

    if (evlist == NULL)
    {
        evlist = malloc(sizeof(struct event_list));
        bzero(evlist, sizeof(struct event_list));
        evlist->name = malloc(strlen(name)+1);
        strcpy(evlist->name, name);
        evlist->index = 0;
        evlist->next = NULL;
        return 0;
    }

    for (struct event_list *e = evlist; e; e = e->next) {
        if (strcmp(name, e->name) == 0) {
            return e->index;
        }
        last_entry = e;
    }

    last_entry->next = malloc(sizeof(struct event_list));
    bzero(last_entry->next, sizeof(struct event_list));
    struct event_list *e = last_entry->next;
    e->name = malloc(strlen(name)+1);
    strcpy(e->name, name);
    e->index = last_entry->index+1;
    return e->index;
}

static void ufsm_gen_regions(struct ufsm_region *region);

static void ufsm_gen_states(struct ufsm_state *state)
{
    fprintf(fp_c,"static struct ufsm_state %s = {\n",id_to_decl(state->id));

    if (flag_strip) {
         fprintf (fp_c,"  .id     = \"\", \n");
         fprintf (fp_c,"  .name   = \"\", \n");
    } else {
        fprintf(fp_c,"  .id   = \"%s\",\n",state->id);
        fprintf(fp_c,"  .name = \"%s\",\n",state->name);
    }
    fprintf(fp_c,"  .kind = %i,\n",state->kind);
    fprintf(fp_c,"  .parent_region = &%s,\n",
                            id_to_decl(state->parent_region->id));
    if (state->entry)
        fprintf(fp_c,"  .entry = &%s,\n",id_to_decl(state->entry->id));
    else
        fprintf(fp_c,"  .entry = NULL,\n");


    if (state->doact)
        fprintf(fp_c,"  .doact = &%s,\n",id_to_decl(state->doact->id));
    else
        fprintf(fp_c,"  .doact = NULL,\n");


    if (state->exit)
        fprintf(fp_c,"  .exit = &%s,\n",id_to_decl(state->exit->id));
    else
        fprintf(fp_c,"  .exit = NULL,\n");

    if (state->region) {
        fprintf(fp_c,"  .region = &%s,\n",id_to_decl(state->region->id));
    } else if (state->submachine) {
        state->submachine->region->parent_state = state;
        fprintf(fp_c,"  .region = &%s,\n",id_to_decl(state->submachine->region->id));
    } else {
        fprintf(fp_c,"  .region = NULL,\n");
    }

    fprintf(fp_c,"  .submachine = NULL,\n");

    if (state->next)
        fprintf(fp_c,"  .next = &%s,\n",id_to_decl(state->next->id));
    else
        fprintf(fp_c,"  .next = NULL,\n");

    fprintf(fp_c,"};\n");
    if (state->region)
        ufsm_gen_regions(state->region);

    for (struct ufsm_entry_exit *e = state->entry; e; e = e->next) {
        fprintf(fp_c, "static struct ufsm_entry_exit %s = {\n",
                        id_to_decl(e->id));
        if (flag_strip) {
             fprintf (fp_c,"  .id     = \"\", \n");
             fprintf (fp_c,"  .name   = \"\", \n");
        } else {
            fprintf(fp_c, "  .id = \"%s\",\n", e->id);
            fprintf(fp_c, "  .name = \"%s\",\n",e->name);
        }
        fprintf(fp_c, "  .f = &%s,\n", e->name);
        if (e->next)
            fprintf (fp_c, "  .next = &%s,\n", id_to_decl(e->next->id));
        else
            fprintf (fp_c, "  .next = NULL,\n");

        fprintf(fp_c, "};\n");

        bool found_duplicate = false;

        for (struct ufsm_entry_exit *ee = eelist_first; ee; ee = ee->next)
        {
            if (strcmp(ee->name,e->name) == 0)
            {
                found_duplicate = true;
                break;
            }
        }

        if (!found_duplicate)
        {
            (*eelist) = malloc(sizeof(struct ufsm_entry_exit));
            memcpy ((*eelist), e, sizeof(struct ufsm_entry_exit));
            eelist = &(*eelist)->next;
        }


    }


    for (struct ufsm_doact *d = state->doact; d; d = d->next) {
        fprintf(fp_c, "static struct ufsm_doact %s = {\n",
                        id_to_decl(d->id));
        if (flag_strip) {
             fprintf (fp_c,"  .id     = \"\", \n");
             fprintf (fp_c,"  .name   = \"\", \n");
        } else {
            fprintf(fp_c, "  .id = \"%s\",\n", d->id);
            fprintf(fp_c, "  .name = \"%s\",\n",d->name);
        }

        fprintf(fp_c, "  .f_start = &%s_start,\n", d->name);
        fprintf(fp_c, "  .f_stop = &%s_stop,\n", d->name);


        if (d->next)
            fprintf (fp_c, "  .next = &%s,\n", id_to_decl(d->next->id));
        else
            fprintf (fp_c, "  .next = NULL,\n");


        fprintf(fp_c, "};\n");

        bool found_duplicate = false;

        for (struct ufsm_doact *da = doact_first; da; da = da->next)
        {
            if (strcmp(da->name,d->name) == 0)
            {
                found_duplicate = true;
                break;
            }
        }

        if (!found_duplicate)
        {
            (*doact_list) = malloc(sizeof(struct ufsm_doact));
            memcpy ((*doact_list), d, sizeof(struct ufsm_doact));
            doact_list = &(*doact_list)->next;
        }

    }

    for (struct ufsm_entry_exit *e = state->exit; e; e = e->next) {
        fprintf(fp_c, "static struct ufsm_entry_exit %s = {\n",
                        id_to_decl(e->id));
        if (flag_strip) {
             fprintf (fp_c,"  .id     = \"\", \n");
             fprintf (fp_c,"  .name   = \"\", \n");
        } else {
            fprintf(fp_c, "  .id = \"%s\",\n", e->id);
            fprintf(fp_c, "  .name = \"%s\",\n",e->name);
        }

        fprintf(fp_c, "  .f = &%s,\n", e->name);
        if (e->next)
            fprintf (fp_c, "  .next = &%s,\n", id_to_decl(e->next->id));
        else
            fprintf (fp_c, "  .next = NULL,\n");


        fprintf(fp_c, "};\n");

        bool found_duplicate = false;

        for (struct ufsm_entry_exit *ee = eelist_first; ee; ee = ee->next)
        {
            if (strcmp(ee->name,e->name) == 0)
            {
                found_duplicate = true;
                break;
            }
        }

        if (!found_duplicate)
        {
            (*eelist) = malloc(sizeof(struct ufsm_entry_exit));
            memcpy ((*eelist), e, sizeof(struct ufsm_entry_exit));
            eelist = &(*eelist)->next;
        }
    }
}

static void ufsm_gen_regions(struct ufsm_region *region)
{
    for (struct ufsm_region *r = region; r; r = r->next) {
        fprintf (fp_c,"static struct ufsm_region %s = {\n",id_to_decl(r->id));
        if (flag_strip) {
             fprintf (fp_c,"  .id     = \"\", \n");
             fprintf (fp_c,"  .name   = \"\", \n");
        } else {
            fprintf (fp_c,"  .id = \"%s\",\n", r->id);
            fprintf (fp_c,"  .name = \"%s\",\n", r->name);
        }
        if (r->state)
            fprintf (fp_c,"  .state = &%s,\n", id_to_decl(r->state->id));
        else
            fprintf (fp_c,"  .state = NULL,\n");

        fprintf (fp_c,"  .has_history = %s,\n", r->has_history ? "true" : "false");
        fprintf (fp_c,"  .history = NULL,\n");
        if (r->transition)
            fprintf (fp_c,"  .transition = &%s,\n",
                                            id_to_decl(r->transition->id));
        else
            fprintf (fp_c,"  .transition = NULL,\n");

        if (r->parent_state)
            fprintf (fp_c,"  .parent_state = &%s,\n",
                                id_to_decl(r->parent_state->id));
        else
            fprintf (fp_c,"  .parent_state = NULL,\n");
        if (r->next)
            fprintf (fp_c,"  .next = &%s,\n",id_to_decl(r->next->id));
        else
            fprintf (fp_c,"  .next = NULL,\n");
        fprintf (fp_c,"};\n");

        for (struct ufsm_transition *t = r->transition; t; t = t->next) {

            if (t->trigger)
            {
                fprintf(fp_c, "static struct ufsm_trigger %s_triggers[] = {\n",
                            id_to_decl(t->id));

                for (struct ufsm_trigger *tt = t->trigger;tt;tt=tt->next)
                {
                    fprintf(fp_c, "{\n");
                    fprintf(fp_c, "  .trigger = %i,\n",
                                ev_name_to_index(tt->name));
                    fprintf(fp_c, "  .name = \"%s\",\n", tt->name);
                    fprintf(fp_c, "},\n");
                }
                    fprintf(fp_c, " {\n");
                    fprintf(fp_c, "  .trigger = -1,\n");
                    fprintf(fp_c, "  .name = NULL,\n");
                    fprintf(fp_c, "},\n");
                fprintf(fp_c,"};\n");
            }

            fprintf(fp_c, "static struct ufsm_transition %s = {\n",
                               id_to_decl(t->id));
            if (flag_strip) {
                 fprintf (fp_c,"  .id     = \"\", \n");
                 fprintf (fp_c,"  .name   = \"\", \n");
            } else {
                fprintf(fp_c, "  .id = \"%s\",\n", t->id);
                fprintf(fp_c, "  .name = \"\",\n");
            }

            if (t->trigger != NULL)
            {
                fprintf(fp_c, "  .trigger = %s_triggers,\n",
                                id_to_decl(t->id));
            }
            else
            {
                fprintf(fp_c, "  .trigger = NULL,\n");
            }

            fprintf(fp_c, "  .kind = %i,\n",t->kind);
            if (t->action) {
                if (strcmp(t->action->name, "ufsm_defer") == 0)
                {
                    fprintf(fp_c, "  .action = NULL,\n");
                    fprintf(fp_c, "  .defer = true,\n");
                }
                else
                {
                    fprintf(fp_c, "  .action = &%s,\n", id_to_decl(t->action->id));
                    fprintf(fp_c, "  .defer = false,\n");

                    bool found_duplicate = false;

                    for (struct ufsm_action *aa = action_first; aa; aa = aa->next)
                    {
                        if (strcmp(aa->name,t->action->name) == 0)
                        {
                            found_duplicate = true;
                            break;
                        }
                    }

                    if (!found_duplicate)
                    {
                        (*action_list) = malloc(sizeof(struct ufsm_action));
                        memcpy ((*action_list), t->action, sizeof(struct ufsm_action));
                        action_list = &(*action_list)->next;
                    }
                }

            } else {
                fprintf(fp_c, "  .action = NULL,\n");
                fprintf(fp_c, "  .defer = false,\n");
            }
            if (t->guard)
            {
                fprintf(fp_c, "  .guard = &%s,\n", id_to_decl(t->guard->id));

                bool found_duplicate = false;

                for (struct ufsm_guard *gg = guard_first; gg; gg = gg->next)
                {
                    if (strcmp(gg->id,t->guard->id) == 0)
                    {
                        found_duplicate = true;
                        break;
                    }
                }

                if (!found_duplicate)
                {
                    (*guard_list) = malloc(sizeof(struct ufsm_guard));
                    memcpy ((*guard_list), t->guard, sizeof(struct ufsm_guard));
                    guard_list = &(*guard_list)->next;
                }
            }
            else
            {
                fprintf(fp_c, "  .guard = NULL,\n");
            }

            fprintf(fp_c, "  .source = &%s,\n",id_to_decl(t->source->id));
            fprintf(fp_c, "  .dest = &%s,\n",id_to_decl(t->dest->id));
            if (t->next)
                fprintf(fp_c, "  .next = &%s,\n",id_to_decl(t->next->id));
            else
               fprintf(fp_c, "  .next = NULL,\n");
            fprintf(fp_c, "};\n");
            for (struct ufsm_action *a = t->action; a; a = a->next) {
                if (strcmp(a->name, "ufsm_defer") == 0)
                    continue;
                fprintf(fp_c, "static struct ufsm_action %s = {\n",
                            id_to_decl(a->id));
                if (flag_strip) {
                     fprintf (fp_c,"  .id     = \"\", \n");
                     fprintf (fp_c,"  .name   = \"\", \n");
                } else {
                    fprintf(fp_c, "  .id = \"%s\",\n", a->id);
                    fprintf(fp_c, "  .name = \"%s\",\n", a->name);
                }
                fprintf(fp_c, "  .f = &%s,\n", a->name);
                if (a->next)
                    fprintf(fp_c, "  .next = &%s,\n", id_to_decl(a->next->id));
                else
                    fprintf(fp_c, "  .next = NULL,\n");
                fprintf(fp_c, "};\n");
            }
            for (struct ufsm_guard *g = t->guard; g; g = g->next) {
                fprintf(fp_c, "static struct ufsm_guard %s = {\n",
                            id_to_decl(g->id));
                fprintf(fp_c, "  .id = \"%s\",\n", g->id);
                fprintf(fp_c, "  .name = \"%s\",\n", g->name);
                fprintf(fp_c, "  .f = &%s,\n", g->name);
                if (g->next)
                    fprintf(fp_c, "  .next = &%s,\n", id_to_decl(g->next->id));
                else
                    fprintf(fp_c, "  .next = NULL,\n");

                fprintf(fp_c, "};\n");
            }
        }
        for (struct ufsm_state *s = r->state; s; s = s->next)
            ufsm_gen_states(s);
    }
}

bool ufsm_gen_machine (struct ufsm_machine *m)
{
    fprintf (fp_c,"static struct ufsm_machine %s = {\n",id_to_decl(m->id));
    if (flag_strip) {
         fprintf (fp_c,"  .id     = \"\", \n");
         fprintf (fp_c,"  .name   = \"\", \n");
    } else {
        fprintf (fp_c,"  .id     = \"%s\", \n", m->id);
        fprintf (fp_c,"  .name   = \"%s\", \n", m->name);
    }
    fprintf (fp_c,"  .region = &%s,    \n",id_to_decl(m->region->id));
    if (m->next)
        fprintf (fp_c,"  .next = &%s, \n", id_to_decl(m->next->id));
    else
        fprintf (fp_c,"  .next = NULL,\n");


    if (m->parent_state)
        fprintf (fp_c,"  .parent_state = &%s, \n", id_to_decl(m->parent_state->id));
    else
        fprintf (fp_c,"  .parent_state = NULL, \n");

    fprintf (fp_c,"};\n");

    if (m->region)
        ufsm_gen_regions(m->region);
    return true;
}



static void ufsm_gen_regions_decl(struct ufsm_region *region);

static void ufsm_gen_states_decl(struct ufsm_state *state)
{
    fprintf(fp_c,"static struct ufsm_state %s;\n",id_to_decl(state->id));
    if (state->region)
        ufsm_gen_regions_decl(state->region);



    for (struct ufsm_entry_exit *e = state->entry; e; e = e->next)
        fprintf(fp_c, "static struct ufsm_entry_exit %s;\n",
                        id_to_decl(e->id));

    for (struct ufsm_doact *e = state->doact; e; e = e->next)
        fprintf(fp_c, "static struct ufsm_doact %s;\n",
                        id_to_decl(e->id));

    for (struct ufsm_entry_exit *e = state->exit; e; e = e->next)
        fprintf(fp_c, "static struct ufsm_entry_exit %s;\n",
                        id_to_decl(e->id));


}

static void ufsm_gen_regions_decl(struct ufsm_region *region)
{
    for (struct ufsm_region *r = region; r; r = r->next) {
        fprintf (fp_c,"static struct ufsm_region %s;\n",id_to_decl(r->id));

        for (struct ufsm_transition *t = r->transition; t; t = t->next) {
            fprintf(fp_c, "static struct ufsm_transition %s;\n",
                               id_to_decl(t->id));

            for (struct ufsm_action *a = t->action; a; a = a->next)
            {
                if (! (strcmp(a->name, "ufsm_defer") == 0))
                    fprintf(fp_c, "static struct ufsm_action %s;\n",
                            id_to_decl(a->id));
            }

            for (struct ufsm_guard *g = t->guard; g; g = g->next)
                fprintf(fp_c, "static struct ufsm_guard %s;\n",
                            id_to_decl(g->id));

        }

        for (struct ufsm_state *s = r->state; s; s = s->next)
            ufsm_gen_states_decl(s);

    }
}

bool ufsm_gen_machine_decl (struct ufsm_machine *m)
{
    fprintf (fp_c,"static struct ufsm_machine %s;\n",id_to_decl(m->id));
    ufsm_gen_regions_decl(m->region);
    return true;
}

bool ufsm_gen_output(struct ufsm_machine *root, char *output_name,
                    char *output_prefix, uint32_t verbose, bool strip)
{
    v = verbose;

    if (v) printf ("o Generating output %s\n", output_name);

    char *fn_c = malloc(strlen(output_name)+strlen(output_prefix)+3);
    char *fn_h = malloc(strlen(output_name)+strlen(output_prefix)+3);

    flag_strip = strip;

    sprintf(fn_c, "%s%s.c",output_prefix,output_name);
    sprintf(fn_h, "%s%s.h",output_prefix,output_name);

    fp_c = fopen(fn_c, "w");
    fp_h = fopen(fn_h, "w");

    if (fp_c == NULL) {
        printf ("Error: Could not open file '%s' for writing\n", fn_c);
        return false;
    }

    if (fp_h == NULL) {
        printf ("Error: Could not open file '%s' for writing\n", fn_h);
        return false;
    }

    fprintf(fp_h,"#ifndef UFSM_GEN_%s_H__\n", output_name);
    fprintf(fp_h,"#define UFSM_GEN_%s_H__\n", output_name);
    fprintf(fp_h,"#include <ufsm.h>\n");
    fprintf(fp_h,"#ifndef NULL\n");
    fprintf(fp_h," #define NULL (void *) 0\n");
    fprintf(fp_h,"#endif\n");



    fprintf(fp_c,"#include \"%s\"\n", fn_h);

    for (struct ufsm_machine *m = root; m; m = m->next)
        ufsm_gen_machine_decl(m);

    fprintf(fp_c,"\n\n\n");
    for (struct ufsm_machine *m = root; m; m = m->next)
        ufsm_gen_machine(m);

    fprintf(fp_c,"\n");
    for (struct ufsm_machine *m = root; m; m = m->next) {
        fprintf(fp_c,"struct ufsm_machine * get_%s(void) { return &%s; }\n",
                    m->name, id_to_decl(m->id));
    }

    for (struct ufsm_entry_exit *ee = eelist_first; ee; ee = ee->next)
        fprintf(fp_h, "void %s(void);\n", ee->name);
    for (struct ufsm_guard *gg = guard_first; gg; gg = gg->next)
        fprintf(fp_h, "bool %s(void);\n",gg->name);
    for (struct ufsm_doact *da = doact_first; da; da = da->next)
    {
        fprintf(fp_h, "void %s_start(struct ufsm_machine *m, struct ufsm_state *s, ufsm_doact_cb_t cb);\n", da->name);
        fprintf(fp_h, "void %s_stop(void);\n", da->name);
    }

    for (struct ufsm_action *aa = action_first; aa; aa = aa->next)
        fprintf(fp_h, "void %s(void);\n",aa->name);

    fprintf(fp_h, "enum {\n");
    for (struct event_list *e = evlist; e; e = e->next) {
        fprintf (fp_h,"  %s,\n",e->name);
    }
    fprintf(fp_h,"};\n");

    for (struct ufsm_machine *m = root; m; m = m->next) {
        fprintf(fp_h,"struct ufsm_machine * get_%s(void);\n",m->name);
    }
    fprintf(fp_h,"#endif\n");
    fclose(fp_c);
    fclose(fp_h);


    return true;
}
