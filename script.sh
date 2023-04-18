if ! test $# -eq 2
then
	echo "Not the right number of arguments"
	exit 1
fi

if ! test -f $1 || test -L $1
then
	echo "Not a regular file!"
	exit 2
fi

if ! [[ $1 == *.c ]]
then
	echo "not a c file"
	exit 3
fi

gcc -Wall -o prog "$1" 2> "$2"

count_errors=$(grep -c "error" "$2")
count_warnings=$(grep -c "warning" "$2")

echo "number of errors: $count_errors"
echo "number of warnings: $count_warnings"

