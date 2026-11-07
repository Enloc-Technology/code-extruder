CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -Werror -O2
LDLIBS ?= -lm

.PHONY: all clean test

all: code-extruder

code-extruder: main.c
	$(CC) $(CFLAGS) $< $(LDLIBS) -o $@

test: code-extruder
	./code-extruder
	./code-extruder --source 'patient=smoke severity=0.2 sulking=false target=java'
	@if ./code-extruder --source 'patient=critical severity=0.9 target=c'; then \
		echo "Expected critical diagnostic to fail"; exit 1; \
	else \
		echo "Critical diagnostic handled as expected"; \
	fi
	@if ./code-extruder --source 'patient=x;'; then \
		echo "Expected malformed source to fail"; exit 1; \
	else \
		echo "Malformed source rejected as expected"; \
	fi

clean:
	rm -f code-extruder
