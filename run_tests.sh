#!/bin/bash
echo "Information Board unit tests"
echo "Compiling tests"
cd tests
for i in `find . -name '*.cpp'`; do
    g++ $i -o "../output/$i.out"
    echo "Compilation $i done"
done
echo "Compilation complete!"
echo "Running tests..."
cd "../output"
for i in `find . -name '*.out'`; do
    echo "testing $i"
    $i
done
read -n 1 -s -r -p "Press any key to exit "
echo ""
exit
