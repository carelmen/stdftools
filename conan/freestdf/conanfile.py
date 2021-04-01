from conans import ConanFile, AutoToolsBuildEnvironment, tools
import os


class LibstdfConan(ConanFile):
    name = "libstdf"
    version = "0.4"
    license = "BSD"
    author = "Mike Frysinger <vapier@gmail.com> "
    url = "https://freestdf.sourceforge.io"
    description = "Free stdf library"
    topics = ("<Put some tag here>", "<here>", "<and here>")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = {"shared": False}
    generators = "cmake"

    def source(self):
        tools.get('https://downloads.sourceforge.net/project/freestdf/libstdf/libstdf-0.4.tar.bz2')


    def build(self):
        autotools = AutoToolsBuildEnvironment(self)
        os.chdir('libstdf-0.4')
        autotools.configure()
        autotools.configure()
        autotools.make()

    def package(self):
        self.copy("*.h", dst="include", keep_path=False) # , src=""
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.dylib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["hello"]

