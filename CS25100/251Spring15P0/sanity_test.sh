javac MaxFind.java
java MaxFind < data/sample_input.txt > data/prog_output.txt

if cmp -s data/prog_output.txt data/sample_output.txt
then
   echo "Output perfect!"
else
   echo "Output Differs. Check your program"
fi

