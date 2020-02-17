include makefile.head

all: $(DIR) $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS)
	@rm -rf $(OBJ_DIR)

$(OBJ_DIR)/%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(DIR):
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)

clean:  
	rm -rf $(OBJ_DIR)
	rm -f $(BIN_DIR)/$(BIN_NAME)
