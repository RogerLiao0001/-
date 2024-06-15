all:
	rm -f ai_script.toml
	gcc main.c cJSON.c -o main -lcurl -lm
	
	gcc test.c cJSON.c -o test -lcurl -lm
	# gcc test2.c cJSON.c -o test2 -lcurl -lm