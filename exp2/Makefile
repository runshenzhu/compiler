Compiler: lex.l syntax.y
	bison -d syntax.y
	flex lex.l
	gcc -o compiler syntax.tab.c main.c Fun.c hash.c -lfl -ly
clean:
	rm -f compiler *.yy.c *.tab.h *.tab.c
