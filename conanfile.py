from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps
import os

class rslXmlRecipe(ConanFile):
    name = "rsl-xml"
    version = "0.1"
    package_type = "library"

    # Optional metadata
    license = "<Put the package license here>"
    author = "<Put your name here> <And your email here>"
    url = "<Package recipe repository url here, for issues about the package>"
    description = "<Description of mypkg package here>"
    topics = ("<Put some tag here>", "<here>", "<and here>")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "examples": [True, False],
        "tests" : [True, False]
    }
    default_options = {"shared": False, "fPIC": True, "examples": False, "tests": False}

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "CMakeLists.txt", "src/*", "include/*", "test/*"

    def config_options(self):
        if self.settings.os == "Windows":
            self.options.rm_safe("fPIC")

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")

    def requirements(self):
        self.requires("rsl-util/0.1", transitive_headers=True)
        self.test_requires("gtest/1.14.0")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure(
            variables={
                "BUILD_TESTING": self.options.tests,
                "BUILD_EXAMPLES": self.options.examples,
            })
        cmake.build()
        cmake.install()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.set_property("cmake_file_name", "rsl-xml")
        self.cpp_info.components["xml"].set_property("cmake_target_name", "rsl::xml")
        self.cpp_info.components["xml"].includedirs = ["include"]
        self.cpp_info.components["xml"].libdirs = ["lib"]
        self.cpp_info.components["xml"].libs = ["rsl_xml"]
        self.cpp_info.components["xml"].requires = ["rsl-util::util"]
