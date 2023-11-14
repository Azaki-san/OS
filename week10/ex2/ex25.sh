#!/bin/bash
touch ex5.txt
chmod a-w ex5.txt

chmod u=rwx,go=rwx ex5.txt

chmod g=u ex5.txt
echo "Permissions for ex5.txt (numeric): $(stat -c '%a' ex5.txt)"
echo "Permissions for ex5.txt (symbolic): $(ls -l ex5.txt | cut -d ' ' -f 1)"

echo "660 means read and write permissions for the owner, read and write permissions for the group, and no permissions for others."
echo "775 means read, write, and execute permissions for the owner and group, and read and execute permissions for others."
echo "777 means read, write, and execute permissions for everyone (owner, group, and others)."

