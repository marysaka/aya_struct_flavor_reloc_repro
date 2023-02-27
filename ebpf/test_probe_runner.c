#include "test_probe.skel.h"
#include <stdio.h>
#include <unistd.h>

int main()
{
    struct test_probe_bpf *skel;
    int err = 0;

    skel = test_probe_bpf__open();
    if (!skel)
            goto cleanup;

    err = test_probe_bpf__load(skel);
    if (err)
        goto cleanup;

    printf("Probe loaded\n");

cleanup:
    test_probe_bpf__destroy(skel);
    return err;
}