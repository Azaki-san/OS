
bash gen.sh 10 ex1.txt

ln ex1.txt ex11.txt
ln ex1.txt ex12.txt

diff ex1.txt ex11.txt
diff ex1.txt ex12.txt

stat --format="%i - %n" ex1.txt ex11.txt ex12.txt > output.txt

du ex1.txt

ln ex1.txt ex13.txt
mv ex13.txt /tmp/

find ./ -inum $(stat -c "%i" ex1.txt)

find / -inum $(stat -c "%i" ex1.txt)

stat -c "%h" ex1.txt

find ./ -inum $(stat -c "%i" ex1.txt) -exec rm {} \;

