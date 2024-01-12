# Semaphore Sandwiches

You are now the new owner to Semaphore Sandwiches, a special sandwich shop that rents out space to actual sandwich makers. Right now you have two businesses leasing the space, the Carnivores who specialize in meat sandwiches and the Vegetarians who specialize in (you guessed it) vegetarian sandwiches. The space you lease out is small, so only one business can be active at a time. In other words, if the Carnivores are currently serving customers, then the Vegetarians cannot make sandwiches and vice versa. 

You are a savvy business person and think that to minimize the number of times the businesses switch kitchens (and thus lose time and money) they should switch only when the number of customers reach a majority. For instance, if there are 5 Carnivore customers and 3 Vegetarian Customers, then the Carnivore customers are served. This could lead to a fast and starvation free switching policy. 

# Necessary Ingredients
Visualize each sandwich worker as a thread who gets an ingredient and then waits for the other ingredients before assembling the sandwich. Thankfully, the two shops use the same bread supplier, but they have a different main ingredient: Meat for the Carnivores and Tofu for the Vegetarians.  Thus, you are tasked with writing the code that coordinates the workers. Each sandwich takes 2 pieces of bread and 1 main ingredient. In order to assemble these ingredients (threads) into sandwiches, we have to create a barrier that makes each thread wait until a complete sandwich is ready to proceed. As each thread passes the barrier they will invoke the function assemble_sandwich. You must guarantee that all threads from one sandwich invoke assemble_sandwich before any of the threads from the next sandwich do. I.e. you must ensure that:
1. If a bread thread arrives at the barrier when no other threads are present, it has to wait for another bread thread, and a main ingredient thread
2. If a main ingredient thread arrives at the barrier when no bread threads are present, then it has to wait for two bread threads

There are 3 files to fill out:
- Bread.h
	- Within bread.h is a stub method called bread_thread. Fill out this thread to replicate the sandwich assembly instructions as above. Be sure to invoke assemble_sandwich
	- bread_thread will be called by a worker thread so be sure to implement the correct concurrency controls -- No one likes too much bread on a sandwich
- Tofu.h
	- Within tofu.h is a stub method called tofu_thread. Fill out this thread to replicate the sandwich assembly instructions as above. Be sure to invoke assemble_sandwich
	- tofu_thread will be called by a worker thread so be sure to implement the correct concurrency controls -- if there is too much or not enough tofu on a sandwhich, the customers will revolt
- Meat.h
	- meat.h is much like tofu.h except the sub method is called meat_thread :)

# Implementing the switching routine
Customers will arrive and check in to indicate which sandwich they want to order. Once a customer checks in their vote counts towards the majority rule. 
There is a file called shared_kitchen.h where the worker threads will be spawned from. Within the shared_kitchen are two stub methods, carnivore_thread and vegetarian_thread as well as a predefined method called init_kitchen. 

- init_kitchen:
	- 
	This is where you will initialize the concurrency controls for the worker threads. You can solve both the worker threads and the orchestration of the worker threads (majority rule) by using semaphores, but that is not the only way. Think carefully about how to orchestrate the workers. 

- carnivore_thread
	- 
	This is where you will implement the logic for the carnivore worker. It is important to respect the vegetarian workers and only start assembling a sandwich when it is the Carnivores in control of the kitchen. You should also invoke assemble_sandwich in this method. 
- vegetarian_thread
	- 
	symmetric with carnivore thread
# Hints
This project can be fully implemented with semaphores, hence the name of your new shop. 

You should probably start with implementing the bread.h, tofu.h and meat.h first. Since the main ingredient is symmetric w.r.t. the sandwich shop, implementing tofu.h is also implementing meat.h (with a change of variables). 

Once you satisfy the actual assembly of the sandwiches, start on the shared_kitchen. It is very easy to get deadlocked in shared_kitchen, so a quick and dirty method to finding if you are deadlocked is to run your program a lot of times -- if it runs for a long time, its **probably** not deadlocking. 

You can split the kitchen into **5** different states/statuses
	    **1. NEUTRAL:** No one is in control of the kitchen, whoever comes first (Carnivore or Vegetarian) gains control of the kitchen
		**2. Carnivore:** The carnivores are in control of the kitchen. Implicitly this means the number of carnivore customers outnumber the number of vegetarian customers
		**3. Vegetarian:** The vegetarians are in control of the kitchen. Implicitly this means the number of vegetarian customers outnumber the number of carnivore customers
		**4. Transition to Carnivore:** This can happen through the following sequence of events. First the kitchen is controlled by the Vegetarians (lets say 8 vegetarians and 6 carnivore). Now 2 carnivores check in, so we have equality. This doesn't change anything yet. Now, if a vegetarian gets their sandwich, they check out and the number becomes 7 V and 8 C. This would trigger a transition. Alternatively, if another carnivore checks in, then the number becomes 8 V and 9 C. This would also trigger a transition
		**5. Transition to Vegetarian:** Like Transition to Carnivore but in reverse.
		
During a transition, all workers are carrying equipment and thus cannot accept new orders (i.e. the customer cannot check in). For example, if we have Transition to Carnivore and have 4 V and 5 C, then a possible sequence is as follows: 

	State = TRANSITION TO CARNIVORE
	Num Carnivores: 5
	Num Vegetarians: 4
	2 Vegetarians get their food. (2 V 5 C)
	A vegetarian tries to check in but has to wait (2 V 5 C)
	A carnivore tries to check in and succeeds (2 V 6 C)
	2 Vegetarians get their food (0 V 6 C)
	The state now changes from Transition to Carnivore to Carnivore (0 V 6 C)
	Now the carnivore workers can assemble the sandwiches and the 
	vegetarian workers can start accepting new orders. 
