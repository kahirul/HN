TARGET ?= hn

BUILD_DIR ?= ./build
SRC_DIR ?= ./src

run: build
	$(BUILD_DIR)/$(TARGET)

build: $(BUILD_DIR)/main.o $(BUILD_DIR)/hn.o $(BUILD_DIR)/client.o
	$(CC) $(BUILD_DIR)/main.o $(BUILD_DIR)/hn.o $(BUILD_DIR)/client.o -o $(BUILD_DIR)/$(TARGET) -ljson-c -lcurl

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.c
	$(CC) -Wall -c $(SRC_DIR)/main.c -o $(BUILD_DIR)/main.o

$(BUILD_DIR)/hn.o: $(SRC_DIR)/hn.c $(SRC_DIR)/hn.h
	$(CC) -Wall -c $(SRC_DIR)/hn.c -o $(BUILD_DIR)/hn.o

$(BUILD_DIR)/client.o: $(SRC_DIR)/client.c $(SRC_DIR)/client.h
	$(CC) -Wall -c $(SRC_DIR)/client.c -o $(BUILD_DIR)/client.o

clean:
	rm $(BUILD_DIR)/*.o $(BUILD_DIR)/$(TARGET)
