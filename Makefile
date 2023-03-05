tt: main.c common.h ini.h irc.h strutil.h boggle.h trie.h
	gcc main.c -o boggle -lm -g -Wall -Wextra -pedantic

clean:
	rm boggle
