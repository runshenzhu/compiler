Compiler: lex.l
	flex lex.l
	gcc -o compiler lex.yy.c -lfl

clean:
	rm -f compiler *.yy.c