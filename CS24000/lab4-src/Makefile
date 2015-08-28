
all: toascii countlines towords rpncalc printargs convert

toascii: toascii.c
	gcc -g -o toascii toascii.c

countlines: countlines.c
	gcc -g -o countlines countlines.c

towords: towords.c
	gcc -g -o towords towords.c

rpncalc: rpncalc.c
	gcc -g -o rpncalc rpncalc.c -lm

convert: convert.c
	gcc -g -o convert convert.c

printargs: printargs.c
	gcc -g -o printargs printargs.c

org: countlines towords rpncalc convert
	cp countlines countlines.org
	cp towords towords.org
	cp rpncalc rpncalc.org
	cp convert convert.org


clean:
	rm -f hello toascii countlines towords printargs rpncalc convert *out
