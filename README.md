# URepair Project
## Created for SENG 3110 at Thompson Rivers Univerity by Trenton, Kyle, and Jordan

# Setup

## Install Visual Studio 2022
[link](https://visualstudio.microsoft.com/vs/)

## Install vcpkg and the libraries
The instructions can be found [here](https://vcpkg.io/en/getting-started.html)
1. Create a directory to install vcpkg (You will probably want something simple and close to the head. I used C:/dev/)
2. Clone the repository into the new directory using `git clone https://github.com/Microsoft/vcpkg.git`
3. Build vcpkg with `.\vcpkg\bootstrap-vcpkg.bat`
4. Change to the vcpkg directory using `cd vcpkg`
5. Install the needed packages with `.\vcpkg install restbed:x64-windows rapidjson:x64-windows`

## Install Node.js and libraries
1. Install [Node.js](https://nodejs.org/en/)
2. 
