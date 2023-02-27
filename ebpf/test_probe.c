#include "vmlinux.h"
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>
#include <bpf/bpf_core_read.h>

struct task_struct_rh___redhat {
    u64 self_exec_id;
};

struct task_struct___redhat {
    struct task_struct_rh___redhat *task_struct_rh;
};

#define printk(fmt, ...) ({                                    \
        char __fmt[] = fmt;                                    \
        bpf_trace_printk(__fmt, sizeof(__fmt), ##__VA_ARGS__); \
})

SEC("tp/bpf_prog")
int
bpf_prog(
    void *ctx
) {
    struct task_struct *task;
    u64 exec_id;

    task = (struct task_struct *)bpf_get_current_task();
    if (!task) {
        return 0;
    }

    // On CentOS 7 kernel, self_exec_id is part of task_struct_rh.
    if (bpf_core_field_exists(((struct task_struct___redhat *)task)->task_struct_rh)) {
        exec_id = BPF_CORE_READ((struct task_struct___redhat *)task, task_struct_rh, self_exec_id);
    } else {
        exec_id = BPF_CORE_READ(task, self_exec_id);
    }

    printk("bpf_prog exec_id: %lx", exec_id);

    return 0;
}

char LICENSE[] SEC("license") = "GPL";
