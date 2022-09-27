# PointzAlive

A little cowboycoded project

## install
Follow instruction on https://vcpkg.io/en/getting-started.html :

in C:\dev

    git clone <url>
    .\vcpkg\bootstrap-vcpkg.bat

    cd vcpkg
    vcpkg integrate install

Then, follow the procedure of your IDE to add vcpkg as a CMake Option:

For Clion

- Linux


    -DCMAKE_TOOLCHAIN_FILE=[vcpkg root]/scripts/buildsystems/vcpkg.cmake

- Windows


    -DCMAKE_TOOLCHAIN_FILE=[vcpkg root]/scripts/buildsystems/vcpkg.cmake

If you want to search additional libraries use

    vcpkg search <library_name>
    vcpkg install <library_name>[<feature>]:<target>

Just as an example for imgui

    vcpkg install imgui[docking-experimental,glfw-binding, opengl3-binding]:x64-windows

## Thanks
The initial template was downloaded from Alberto Foti's excellent Imgui template]
