
![](https://github.com/eosswedenorg/antelope-keygen/workflows/CI/badge.svg)
[![GitHub release](https://img.shields.io/github/v/release/eosswedenorg/antelope-keygen?include_prereleases)](https://github.com/eosswedenorg/antelope-keygen/releases/latest)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

# Antelope Keygen

This program generates public and private keypair for [Antelope IO](https://antelope.io)

## Compile

You will need `libantelope` and `cmake 3.15` or later to compile this project.

`Qt 5.9.0` or later is required for the graphical program.

### Linux/MacOS

**NOTE:** Only Ubuntu 20.04 and 22.04 and Fedoora 36 is officially supported.

The project should compile fine on most versions/distros but it is only tested
and distributed for those distros/versions by [Sw/eden](www.eossweden.org).

#### Dependencies

#### Linux

**Ubuntu:**

First you need to have a compiler. this can be installed with apt.

```sh
$ apt-get install gcc g++
```

**CMake**

If your package manager don't provide a sufficiently new version of cmake, you can install it with these commands:

Ubuntu 16.04/18.04 - [kitware](https://apt.kitware.com) ppa:

```sh
$ wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | sudo apt-key add -
$ sudo apt-add-repository "deb https://apt.kitware.com/ubuntu/ `lsb_release -cs` main"
$ sudo apt-get install cmake
```

shell script:

```sh
$ wget -O cmake.sh https://github.com/Kitware/CMake/releases/download/v3.15.5/cmake-3.15.5-Linux-x86_64.sh && sudo sh ./cmake.sh --prefix=/usr/local --skip-license
```

Other methods is documanted at https://cmake.org/download

**Qt (only for gui program)**

You will need to install `qt >= 5.9.0`

On **ubuntu** you can do this via `apt`

```
$ apt-get install qt5-default
```

For other distributions, download the official [qt-installer](https://www.qt.io/download-qt-installer).
It is recommended to install the latest stable version.

#### MacOS

**NOTE:** Support for MacOS is still experimental and may or may not work as expected.

You must have a compiler installed. This project is known to build with `Xcode 11.0` but other versions should work.

You need to have cmake installed also, this can be done with this `brew` command:
```sh
$ brew install cmake
```

If you need a newer version of cmake, you can download the official `.dmg` file: [cmake-3.15.7-Darwin-x86_64.dmg](https://github.com/Kitware/CMake/releases/download/v3.15.7/cmake-3.15.7-Darwin-x86_64.dmg). or see https://cmake.org/download for other versions.

`libantelope` needs to be compiled and installed from source. [Go here](https://github.com/eosswedenorg/libantelope)

**Qt (only for gui program)**

This can be installed with brew.

```sh
$ brew install qt
```

#### Build

Run `./build.sh` to trigger cmake.

If you dont want to use the script. you can build with cmake using the following commands:

```sh
$ mkdir build && cd build
$ cmake .. && make
```

**MacOS:** You may need to point `cmake` to `openssl` by passing the argument
`-D OPENSSL_ROOT_DIR=/usr/local/opt/openssl@1.1` if openssl is not under `/usr/local/opt/openssl@1.1` you need to find the correct path.

### Windows

#### Dependencies

Download and install `cmake` version `3.15` or newer from [cmake.org](https://cmake.org)

You will also need a compiler. [Build Tools for Visual Studio 2019](https://visualstudio.microsoft.com/thank-you-downloading-visual-studio/?sku=BuildTools&rel=16) (Selecting C++ during installation) is recommended.

**Qt (only for gui program)**

Download the official [qt-installer](https://www.qt.io/download-qt-installer) and follow the steps. It is recommended to install the latest stable version.

#### Build.

you need to set `OPENSSL_ROOT_DIR` to the directory where you unpacked
`openssl-1.1.1e-dev.zip` append `x86` if you are on 32-bit system, `x64` for 64-bit.

**NOTE:** `cmake` uses forward slash `/` for path even for windows. so make sure you use that when setting `OPENSSL_ROOT_DIR`

For example:

```
C:\repo> cmake -D OPENSSL_ROOT_DIR="C:/path/to/openssl-1.1/x86" -B build
C:\repo> cmake --build build --config Release
```

**Qt**

If you are compiling the gui program. you will need to point `cmake` to the location `qt` where installed. this can be done by the `CMAKE_PREFIX_PATH` variable:

```
C:\repo> cmake -D CMAKE_PREFIX_PATH="C:/path/to/qt/msvc2017_64"
```

## Compile options

These compile options are available:

| Cmake                      | build.sh          | Description                               |
|--------------------------- | ----------------- | ----------------------------------------- |
| -DCOMPONENT_CLI=`OFF`      | --no-cli          | Do not build cli program                  |
| -DCOMPONENT_GUI=`ON`       | --gui             | Build gui program (Qt5)                   |
| -DUSE_THREADS=`OFF`        | --disable-threads | Disable thread support                    |
| -DCMAKE_BUILD_TYPE=`value` | -t `value`        | Type of build                             |
| -DFORCE_ANSI=`ON`          | --force-ansi      | Force ANSI console colors even on windows |

For more details about options run `./build.sh -l` or `mkdir build && cmake build -LA`

### libantelope

To speed up the build process, you can install `libantelope`

#### Ubuntu

You can use [Sw/eden's APT Repository](https://eosswedenorg.github.io/apt) like this:

```sh
$ sudo apt-get install software-properties-common
$ curl https://apt.eossweden.org/key 2> /dev/null | sudo apt-key add -
$ sudo apt-add-repository -y 'deb [arch=amd64] https://apt.eossweden.org/main `lsb_release -cs` stable'
$ sudo apt-get install libantelope-dev
```
or manually via `.deb` file from [github](https://github.com/eosswedenorg/libantelope/releases)

```sh
$ wget <url>
$ sudo apt install ./libantelope-dev-<version>.deb
```

#### Other

Consult [libantelope's github](https://github.com/eosswedenorg/libantelope)

## Install

After the project has been compiled. run `sudo ./install.sh` or the following code if you dont want to use that:

```sh
# inside the build directory
$ sudo make install
```

**Windows:**

It is possible to run `cmake --install .` from `build` directory.
Your DOS shell needs administrator privileges.

## Uninstall

Run `sudo ./uninstall.sh` or remove the files listed in `build/install_manifest.txt` manually.

## Security notice

Keys are generated by `OpenSSL`'s `EC_KEY_generate_key` function. The program will
never expose your keys to anything but the computers memory and output of the
program. You are free to inspect the source code and compile yourself to verify.

However, use this at your own risk. we cannot guarantee that the keys are
cryptographically secure as this depends on OpenSSL's implementation (alto it is
widely used and should be safe)

Please read the `LICENSE` file.

```
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
```

## Author

Henrik Hautakoski - [Sw/eden](https://eossweden.org/) - [henrik@eossweden.org](mailto:henrik@eossweden.org)
