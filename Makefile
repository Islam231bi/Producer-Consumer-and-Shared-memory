all: producer consumer
	@echo "done"
PRODUCER:
	g++ -g -Wall -o producer producer.cpp
CONSUMER:
	g++ -g -Wall -o consumer consumer.cpp