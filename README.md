# URepair Project
## Created for SENG 3110 at Thompson Rivers Univerity by Trenton, Kyle, and Jordan

# Setup

## Install Visual Studio 2022
1. [link](https://visualstudio.microsoft.com/vs/)
2. Select the `Desktop development with C++` option and install

## Install vcpkg and the libraries
The instructions can be found [here](https://vcpkg.io/en/getting-started.html)
1. Create a directory to install vcpkg (You will probably want something simple and close to the head. I used C:/dev/)
2. Clone the repository into the new directory using `git clone https://github.com/Microsoft/vcpkg.git`
3. Build vcpkg with `.\vcpkg\bootstrap-vcpkg.bat`
4. Change to the vcpkg directory using `cd vcpkg`
5. Install the needed packages with `.\vcpkg install restbed:x64-windows rapidjson:x64-windows`
6. Run `.\vcpkg integrate install`

## Install Node.js + libraries and start web app
1. Install [Node.js](https://nodejs.org/en/)
2. Download this repo and change the active directory into URepair/urepair-app/
3. Install the packages with `npm install axios react formik`
4. Make sure the existing `package-lock.json` file is deleted
5. Start the app with `npm start`

## Start API
1. Create a new project from existing files in Visual Studio
2. Select the API folder from the repository download
3. Run the `jobsdb.cpp` file

