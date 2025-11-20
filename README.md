# CxObfuscator 🛡️

> A lightweight, header-only C++17 library for compile-time string obfuscation and anti-static analysis.

![Language](https://img.shields.io/badge/Language-C%2B%2B17-00599C?style=flat&logo=c%2B%2B)
![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux-lightgrey)
![License](https://img.shields.io/badge/License-MIT-green)
![Memory](https://img.shields.io/badge/Memory-Stack%20Allocated-orange)

## 🔎 Overview

**CxObfuscator** is a developer tool designed to protect sensitive string literals from static analysis tools (such as IDA Pro, Ghidra, or simply `strings`). 

By leveraging C++ **Template Metaprogramming** and `constexpr` logic, this library encrypts strings during the compilation phase. The plain-text strings are never stored in the binary's `.rdata` section, making signature-based detection and static reverse engineering significantly harder.

## ✨ Key Features

*   **Compile-Time Encryption**: Strings are XOR-encoded before the binary is even linked.
*   **Polymorphic Builds**: Utilizes the `__TIME__` macro to generate a unique encryption key for every build. Two compilations of the same code will result in different binary signatures.
*   **Zero Heap Allocation**: Decrypted strings are stored in a temporary `std::array` on the **stack**. This avoids heap fragmentation and ensures data is automatically cleared when it goes out of scope.
*   **Header-Only**: Easy integration. No external libraries or complex build setups required.
*   **Wide Character Support**: Full support for `std::string` and `std::wstring`.

## ⚙️ Technical Deep Dive

This project demonstrates low-level memory manipulation and modern C++ capabilities:

1.  **Key Generation**: An FNV-1a hash algorithm is executed at compile-time to create a seed based on the compilation timestamp.
2.  **Obfuscation**: The target string is processed byte-by-byte using the generated key. The compiler places only the *encrypted* bytes into the executable.
3.  **Runtime Behavior**: 
    *   When the `sk()` macro is called, a lambda constructs the decryptor on the stack.
    *   The decryption routine runs, returning a pointer to the stack buffer.
    *   Once the statement finishes, the stack frame is invalidated, leaving minimal trace of the plain text in memory.

## 🚀 Usage

Simply include the header file and wrap your sensitive strings with the `sk()` macro.

```cpp
#include <iostream>
#include "cxobfuscator.h"

int main() {
    // STANDARD STRING
    // "Secret_API_Key" will NOT appear in the binary file.
    std::cout << "Key: " << sk("Secret_API_Key") << std::endl;

    // WIDE STRING
    // Supports wchar_t for Windows API calls or Unicode.
    std::wcout << L"Log: " << sk_w(L"System_Initialized_Hidden") << std::endl;

    return 0;
}
