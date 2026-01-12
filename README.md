# StewardKL

StewardKL is an experimental Windows input monitoring tool developed for educational and research purposes. It explores low-level keyboard event handling using global keyboard hooks and DLL injection in controlled, consent-based environments.

## Overview

The project is designed to study how keyboard input is processed within the Windows operating system. It separates input collection from visualization to maintain a clean and modular architecture.

## Architecture

- Injected DLL captures keyboard events using a global keyboard hook
- Events are sent to the GUI application via Windows named pipes
- GUI displays events using an ImGui-based interface

## Technology Stack

- Dear ImGui
- DirectX 11
- Win32 API

## Usage Policy

This project is intended strictly for educational and research use. Explicit user consent is required before any input monitoring. Unauthorized or malicious use is prohibited.

## Author

Developer: un4ckn0wl3z
