FLAGS = -Wall -Werror
sandwich: test_sandwich.c bread.h tofu.h meat.h global.h barrier.h
	gcc $(FLAGS) -o test_sandwiches test_sandwiches.c

kitchen: bread.h tofu.h meat.h global.h barrier.h shared_kitchen.h shared_kitchen.c
	gcc $(FLAGS) -o test_kitchen shared_kitchen.c

clean:
	rm test_kitchen
	rm test_sandwich
	rm test_sandwiches