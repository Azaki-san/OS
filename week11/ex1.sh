#!/bin/bash
# Set file size and name variables
file_size="50M"
file_name="lofs.img"

# Create a sparse file with the specified size
dd if=/dev/zero of="$file_name" bs=1 count=0 seek="$file_size"

# Find and show the first available loop device for the file
loop_device=$(losetup --find --show "$file_name")

# Create an ext4 file system on the loop device
mkfs.ext4 "$loop_device"

# Set the mount point for the loop device
mount_point="./lofsdisk"

# Create the mount point directory if it doesn't exist
mkdir -p "$mount_point"

# Mount the loop device on the specified mount point
mount "$loop_device" "$mount_point"

# Display a message indicating that the mount was successful
echo "Mounted $mount_point"

# Create and write content to two files within the mounted filesystem
echo "Andrew" > "$mount_point/file1"
echo "Alexeev" > "$mount_point/file2"

# Define a function to get shared library dependencies for a given command
get_libs() {
    ldd $1 | awk '/=>/ {print $3}'
}

# Copy necessary binary files and their dependencies to the mounted filesystem
for cmd in "bash" "cat" "echo" "ls"; do
    sudo cp $(which $cmd) lofsdisk/
    get_libs $(which $cmd) | xargs -I {} sudo cp {} lofsdisk/
done

# Compile an executable "ex1" with static linking
gcc --static -o ex1 ex1.c

# Copy the compiled executable to the mounted filesystem
sudo cp ex1 lofsdisk

# Perform chroot to the mounted filesystem and run the executable, redirecting output to ex1.txt
sudo echo 'First run:' > ex1.txt
sudo sh -c "chroot lofsdisk /ex1 >> ex1.txt"

# Run the executable from the current environment and append output to ex1.txt
sudo echo 'Second run:' >> ex1.txt
sudo ./ex1 >> ex1.txt

