TARGETS = queue stack duplicates

build: $(TARGETS)

stack: Stack.c
	gcc Stack.c -o stack

queue: Queue.c
	gcc Queue.c -o queue

duplicates: duplicates_stack.c
	gcc duplicates_stack.c -g -o duplicates

run-stack:
	./stack

run-queue:
	./queue

run-duplicates:
	./duplicates

clean:
	rm -f *.class $(TARGETS) *.o