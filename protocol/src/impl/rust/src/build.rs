use std::path::PathBuf;



fn main() {
    #[cfg(debug_assertions)]
    println!("cargo:rustc-link-lib=dylib=asan");

    let manifest = PathBuf::from(std::env::var("CARGO_MANIFEST_DIR").unwrap());
    // rust -> impl -> src -> protocol
    let lib_dir = manifest.join("../../../build/lib").canonicalize().unwrap();

    println!("cargo:rustc-link-search=native={}", lib_dir.display());
    println!("cargo:rustc-link-lib=dylib=socket");
    println!("cargo:rustc-link-lib=dylib=proto");
    println!("cargo:rustc-link-arg=-Wl,-rpath,{}", lib_dir.display());
    println!("cargo:rerun-if-changed={}", lib_dir.display());
}
