# PointzAlive

A little and simple phisics game, made for fun. <br>

<p align="center">
    <img src="Images/Img.png" width="600">
</p>


## installation
### Windows


### Build on Linux using CMake (not tested yet)
- Download and install [vcpkg](https://vcpkg.io/en/getting-started.html). Then:

```
    git clone <this project>
    cd PointzAlive
    mkdir build
    cmake -DCMAKE_TOOLCHAIN_FILE=C:\Users\Thoma\dev\vcpkg\scripts\buildsystems\vcpkg.cmake ..
    make
    cd ..
```
When building is complete, to run it:
```
    make run
```

## Thanks
The initial template for this repository was downloaded from Alberto Foti's excellent [Imgui template](https://github.com/XXmorpheusX/ImGuiTemplate)
