TARGET ?= hn

BUILD_DIR ?= ./build
SRC_DIR ?= ./src

run: build
	$(BUILD_DIR)/$(TARGET)

build: $(BUILD_DIR)/main.o $(BUILD_DIR)/ui.o $(BUILD_DIR)/fetcher.o $(BUILD_DIR)/hn.o $(BUILD_DIR)/client.o
	$(CC) $(BUILD_DIR)/main.o $(BUILD_DIR)/ui.o $(BUILD_DIR)/fetcher.o $(BUILD_DIR)/hn.o $(BUILD_DIR)/client.o -o $(BUILD_DIR)/$(TARGET) -ljson-c -lcurl -lpthread -lncurses

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.c
	$(CC) -g -Wall -Wextra -std=c17 -Wpedantic -c $(SRC_DIR)/main.c -o $(BUILD_DIR)/main.o

$(BUILD_DIR)/ui.o: $(SRC_DIR)/ui.c $(SRC_DIR)/ui.h
	$(CC) -g -Wall -Wextra -std=c17 -Wpedantic -c $(SRC_DIR)/ui.c -o $(BUILD_DIR)/ui.o

$(BUILD_DIR)/fetcher.o: $(SRC_DIR)/fetcher.c $(SRC_DIR)/fetcher.h
	$(CC) -g -Wall -Wextra -std=c17 -Wpedantic -c $(SRC_DIR)/fetcher.c -o $(BUILD_DIR)/fetcher.o

$(BUILD_DIR)/hn.o: $(SRC_DIR)/hn.c $(SRC_DIR)/hn.h
	$(CC) -g -Wall -Wextra -std=c17 -Wpedantic -c $(SRC_DIR)/hn.c -o $(BUILD_DIR)/hn.o

$(BUILD_DIR)/client.o: $(SRC_DIR)/client.c $(SRC_DIR)/client.h
	$(CC) -g -Wall -Wextra -std=c17 -Wpedantic -c $(SRC_DIR)/client.c -o $(BUILD_DIR)/client.o

clean:
	rm $(BUILD_DIR)/*.o $(BUILD_DIR)/$(TARGET)
