#!/bin/bash

# Variables
REPO_OWNER="itsukitatsuya11"
REPO_NAME="TinyHttp"
INSTALL_DIR=""
USER_BIN_DIR="$HOME/.local/bin"
SYS_BIN_DIR="/usr/local/bin"
PROGRAM_NAME="TinyHttp"
VERSION="v1.0"

# Banner
echo "======================================"
echo "  $PROGRAM_NAME Installer $VERSION"
echo "======================================"

# Check if ~/.local/bin directory is available
if [ -d "$USER_BIN_DIR" ]; then
    INSTALL_DIR="$USER_BIN_DIR"
    echo "Using directory $INSTALL_DIR for installation."
else
    # If not, use /usr/local/bin
    INSTALL_DIR="$SYS_BIN_DIR"
    echo "Using directory $INSTALL_DIR for installation."
fi

# Installation Process
echo "======================================"
echo "        Cloning Progress"
echo "======================================"

git clone https://github.com/$REPO_OWNER/$REPO_NAME.git
cd $REPO_NAME
cp tinyhttp $INSTALL_DIR

# Remove installation script and project directory
cd ..
rm -rf $REPO_NAME

echo "======================================"
echo "          Done Progress"
echo "======================================"

# Inform user that installation is complete
echo "Installation complete. You can run the program using the 'tinyhttp' command."

