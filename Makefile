CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2
BIN      := bin

all: $(BIN)/sms $(BIN)/procedural

$(BIN):
	mkdir -p $(BIN)

$(BIN)/sms: src/main.cpp | $(BIN)
	$(CXX) $(CXXFLAGS) -o $@ $<

$(BIN)/procedural: src/procedural_version.cpp | $(BIN)
	$(CXX) $(CXXFLAGS) -o $@ $<

run: $(BIN)/sms
	./$(BIN)/sms

compare: $(BIN)/procedural
	./$(BIN)/procedural

clean:
	rm -rf $(BIN)

.PHONY: all run compare clean
