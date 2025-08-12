# Image Steganography v1.0 (updated for Windows)

A console-based program that was a semester project for a C++ course at PJATK Univesity.

The program mainly works with BMP and PPM picture formats. It supports stated formats for checking properties, encoding and decoding messages into pictures. It also has the ability to check whether a message can be written into an image.
**Currently, this version is undergoing refactoring, therefore the encoding and decoding functions are temporarily unavailable.**

### Important Note before executing command lines
After multiple testing of the program there were many issues that had fixed. **The most important one was related to MinGW**. Since the most of the people don't have MinGW in their PATH, this causes problems that prevent you from seeing the output from program in your PowerShell.

**Therefore**, it was decided to attach two important commands that will be able to point mingw to the path folder only during one session in the terminal.

```powershell
$mingw = 'C:\Program Files\JetBrains\CLion 2024.2.2\bin\mingw\bin'
$env:Path = "$mingw;$env:Path"    
```

Both command lines **must be entered** right before running the ImageSteganography program commands.

### List of commands:

        -h, --help  [no args]                       list of possible commands

        -i, --info  [filepath]                      checks if the path leads to a file in a supported format

        -c, --check [filepath] [string text]        checks if a message could be saved in the given file

        -e, --encrypt [filepath] [string text]      writes into the image file the specified message
                                                    adds to the output file name prefix "enc_[filepath]"

        -d, --decrypt [filepath]                    reads message from encrypted image file
