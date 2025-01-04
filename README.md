# WIFI-QR-Scanner

## Overview
WIFI-QR-Scanner is a desktop application that allows users to connect to Wi-Fi networks by scanning QR codes using a PC camera. The application provides a seamless and user-friendly experience, leveraging advanced technologies for efficient QR code reading and network connection.

## Features
- **QR Code Scanning**: Uses the ZXing-cpp library to read QR codes from the camera feed.
- **Multithreaded Processing**: Processes video frames using a thread pool for enhanced speed and responsiveness.
- **Automatic Network Connection**: Connects to Wi-Fi networks identified in QR codes via system commands (`netsh`).
- **User-Friendly Interface**: Built with QML for an intuitive user experience.

## Technical Stack
- **Qt**: Framework for application development and UI design.
- **QML**: Language for creating responsive user interfaces.
- **C++**: Programming language for backend logic.
- **ZXing-cpp**: Library for QR code scanning functionality.

## Installation
1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/WIFI-QR-Scanner.git
