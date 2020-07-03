#include <stdio.h>
#include <sotc/sotc.h>
#include <sotc/model.h>

#include "render.h"

int main(int argc, char **argv)
{
    int rc;
    struct sotc_model *model;

    rc = sotc_model_load(argv[1], &model);

    if (rc != SOTC_OK)
    {
        printf("Could not load model\n");
        goto err_out;
    }

    printf("Clean-up...\n");
    sotc_model_free(model);

err_out:
    return rc;
}
