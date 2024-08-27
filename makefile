CC = gcc
CFLAGS = -Wall -Wextra -Werror

BIN = bin
SUBPROJECTS = mmcopier

.PHONY: all test clean

all:
	@mkdir -p $(BIN)
	@for proj in $(SUBPROJECTS); do 								 \
		make -C $$proj CC="$(CC)" CFLAGS="$(CFLAGS)";  \
		ln -rsf $$proj/$(BIN)/$$proj $(BIN)/$$proj; 	 \
	done;

clean: $(patsubst %,clean-%,$(SUBPROJECTS))
	rm -rf $(BIN)

clean-%:
	@$(MAKE) -C $* clean
