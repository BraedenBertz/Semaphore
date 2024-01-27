FLAGS = -Wall -Werror -g
kitchen: bread.h tofu.h meat.h global.h shared_kitchen.h shared_kitchen.c
	gcc $(FLAGS) -o test_kitchen shared_kitchen.c

clean:
	rm test_kitchen
	rm test_sandwiches