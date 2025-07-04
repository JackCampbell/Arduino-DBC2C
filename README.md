Arduino-DBC2C CAN Communication Code Generator

This application reads DBC files and generates code for CAN communication on the Arduino Feather M4 board.
It was developed specifically for [MarineMods](https://marinemods.us).

Features
--------

- **DBC File Parsing and Storage**  
  The application parses the DBC files and uses the signal lists and channel information for reuse in subsequent code generation.

- **Custom Code Generation**  
  Based on user input, the generated code can read incoming CAN messages, extract signal data, or send messages/signals over the CAN bus.

- **Cross-Platform GUI**  
  Built using wxWidgets, the application runs on multiple platforms including Windows, Linux, and macOS.

- **Integrated DBC Library Viewer**  
  View detailed attributes, value tables, messages, and signals from the loaded DBC file. The bit mapping of signals within messages is also visualized.

Prerequisites
-------------

- CMake (version 3.10 or higher recommended)  
- A compatible C++ compiler (e.g., GCC, Clang, MSVC)  
- wxWidgets 3.2 and above library is installed on your system.
- Arduino Feather M4 board and Arduino compiler for distribution.


Supported platforms
-------------------

Arduino-DBC2C is a cross-platform C++ toolkit and officially supports:

 - Windows
 - Linux
 - macOS

## Building the Project

To build the application, clone the repository and run the following commands in your terminal:

> You must compile with MSVC's native command from a Windows-based computer.

```bash
git clone https://github.com/JackCampbell/Arduino-DBC2C
cd Arduino-DBC2C
cmake -S . -B ./build -DCMAKE_POLICY_VERSION_MINIMUM=3.5
cmake --build ./build --config Release
```

License
-------

This project is licensed under the MIT License. See the [LICENSE](https://github.com/JackCampbell/Arduino-DBC2C/blob/master/LICENSE) file for details.

Releases
--------

Download the latest release from the [Releases](https://github.com/JackCampbell/Arduino-DBC2C/releases) page.

Support and Contact
-------------------

For support or inquiries, please contact MarineMods at: [info@marinemods.us]

Useful links
------------

* [DBC File Format Documentations](https://nusolar.github.io/training-f23/pdfs/DBC_File_Format_Documentation.pdf)
* [DBC2C](https://jackcampbell.github.io/dbc.html)
* [wxWidgets](https://wxwidgets.org)
* [CodeEd](https://jackcampbell.github.io/codeed)

Developer
---------

* [JackCampbell](https://jackcampbell.github.io)
* [MarineMods](https://marinemods.us)
