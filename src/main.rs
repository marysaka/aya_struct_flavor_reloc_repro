use aya::{*, programs::TracePoint};

fn main() {
    let btf = Btf::from_sys_fs().unwrap();

    let mut test_probe = BpfLoader::new()
                        .btf(Some(&btf))
                        .load_file("ebpf/build/test_probe.bpf")
                        .unwrap();

    let program: &mut TracePoint = test_probe.program_mut("bpf_prog").unwrap().try_into().unwrap();
    program.load().unwrap();
    println!("Probe loaded");
}
