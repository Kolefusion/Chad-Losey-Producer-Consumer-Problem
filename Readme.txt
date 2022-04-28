This project demonstrates a producer writing data to a buffer, while
a consumer reads data from the buffer. The buffer has a size of 2.

To run:
g++ -o producer producer.cpp -pthread -lrt
g++ -o consumer consumer.cpp -pthread -lrt
./producer & ./consumer &
