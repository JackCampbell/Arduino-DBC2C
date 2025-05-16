# CodeEd Project
from waflib import Utils
from waflib.Configure import conf, ConfigurationContext, Context
from waflib.Options import OptionsContext
from waflib.Build import BuildContext, CleanContext, InstallContext, UninstallContext
from waflib.Context import Context, STDOUT, STDERR
import os

def options(opts: OptionsContext):
	opts.load("compiler_c compiler_cxx clang_compilation_database")
	pass

def configure(config: ConfigurationContext):
	config.load("compiler_c compiler_cxx clang_compilation_database")
	config.check_cfg(path="wx-config", args="--libs std,stc --cxxflags", package="", uselib_store="wxWidgets")
	config.env.append_value("CXXFLAGS", ["-std=gnu++14", "-Wno-nonportable-include-path", "-Wno-unused-variable", "-Wno-unused-variable", "-Wno-delete-incomplete"])
	config.env.append_value("LIBDIR", ["/opt/homebrew/lib"])
	config.env.append_value("INCLUDES", ["/opt/homebrew/include"])
	
	# config.setenv("debug", env=config.env.derive())
	config.env.append_value("CXXFLAGS", ["-g", "-Wall"]) # ggdb
	config.define("DEBUG", 1);
	
	# config.setenv("release", env=config.env.derive())
	# config.env.append_value("CXXFLAGS", ["-O2", "-Wall"])
	# config.define("NDEBUG", 1);
	pass

# https://waf.io/book/#_fundamental_waf_commands
def build(build: BuildContext):
	source_dir = build.path.abspath()
	build.program(
		source = build.path.ant_glob("*.cpp") + build.path.ant_glob("stubs/*.cpp"),
		includes = [ source_dir, os.path.join(source_dir, "stubs") ],
		variant = "debug",
		use = [ "wxWidgets" ],
		target = "MarineFeather")
	pass

