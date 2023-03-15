rm CodeCraft-2023.zip
rd /s /q build
mkdir build
cd build
cmake ..
cmake --build .
cd ..
CodeCraft_zip.bat