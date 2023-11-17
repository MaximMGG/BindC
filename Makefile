TARGET = bin
CC = gcc

PR_SRC = ./src/
PR_OBJ = ./obj/

SRC = $(wildcard $(PR_SRC)*.c)
OBJ = $(patsubst $(PR_SRC)%.c, $(PR_OBJ)%.o, $(SRC))

$(TARGET) : $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) 

$(PR_OBJ)%.o : $(PR_SRC)%.c
	$(CC) -c $< -o $@ 

clean : 
	rm $(PR_OBJ)*.o $(TARGET)

debug :  
	$(CC) $(SRC) -g 

deploy:
	sudo cp $(TARGET) /usr/local/sbin/
