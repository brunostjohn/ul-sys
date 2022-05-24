use std::path::PathBuf;
use std::process::Command;
use std::{env, fs};

fn main() {
    let out_dir = PathBuf::from(env::var("OUT_DIR").unwrap());
    let ultralight_dir = out_dir.join("Ultralight");

    println!("cargo:rerun-if-changed=build.rs");
    println!("cargo:rerun-if-changed=wrapper");

    if ultralight_dir.is_dir() {
        fs::remove_dir_all(&ultralight_dir)
            .expect("Could not remove already existing Ultralight repo");
    }

    let git_status = Command::new("git")
        .args(&["clone", "https://github.com/ultralight-ux/Ultralight"])
        .current_dir(&out_dir)
        .status()
        .expect("Git is needed to retrieve the ultralight C++ library!");

    assert!(git_status.success(), "Couldn't clone Ultralight library");

    let git_status = Command::new("git")
        .args(&[
            "reset",
            "--hard",
            "36726f76a13fd0c3416a3cb2b2b323a101c00f2a",
        ])
        .current_dir(&ultralight_dir)
        .status()
        .expect("Git is needed to retrieve the ultralight C++ library!");

    assert!(
        git_status.success(),
        "Could not reset git head to desired revision"
    );

    let dst = cmake::build(ultralight_dir.join("packager"));

    println!(
        "cargo:rustc-link-search=native={}",
        dst.join("bin").display()
    );

    println!("cargo:rustc-link-lib=dylib=Ultralight");
    println!("cargo:rustc-link-lib=dylib=WebCore");
    println!("cargo:rustc-link-lib=dylib=AppCore");

    let bindings = bindgen::Builder::default()
        .header("wrapper/wrapper.h")
        .impl_debug(true)
        .impl_partialeq(true)
        .generate_comments(true)
        .generate_inline_functions(true)
        .allowlist_var("^UL.*|JS.*|ul.*|WK.*")
        .allowlist_type("^UL.*|JS.*|ul.*|WK.*")
        .allowlist_function("^UL.*|JS.*|ul.*|WK.*")
        .generate()
        .expect("Unable to generate bindings");

    let out_path = PathBuf::from(env::var("OUT_DIR").unwrap());

    bindings
        .write_to_file(out_path.join("bindings.rs"))
        .expect("Couldn't write bindings!");
}
