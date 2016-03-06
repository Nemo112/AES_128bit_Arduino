#!/bin/bash
# txt test
echo "" > desten.txt;
echo "" > test.txt;
./CallSerial.py -e -i source.txt -o desten.txt -p /dev/ttyUSB0
./CallSerial.py -d -i desten.txt -o test.txt  -p /dev/ttyUSB0
diff test.txt source.txt && echo "Yep, work fine!" || echo "Doesn't match";
# gif test
echo "" > desten.gif;
echo "" > test.gif;
./CallSerial.py -e -i source.gif -o desten.gif -p /dev/ttyUSB0
./CallSerial.py -d -i desten.gif -o test.gif  -p /dev/ttyUSB0
diff test.gif source.gif && echo "Yep, work fine!" || echo "Doesn't match" ;
display test.gif
