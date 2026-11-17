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
	@./code-extruder --source '' 2>&1 | grep -q '^DEFAULTS_USED=0x0F$$'
	@./code-extruder --source '   ' 2>&1 | grep -q '^DEFAULTS_USED=0x0F$$'
	@./code-extruder --source 'patient=partial' 2>&1 | grep -q '^DEFAULTS_USED=0x0E$$'
	@./code-extruder --source-append 'patient=legacy' 'severity=0.2' 'target=java' 'sulking=false' 2>&1 | grep -q '^Patient: legacy$$'
	@if ./code-extruder --source-append 'patient=first' 'patient=second'; then \
		echo "Expected duplicate appended fields to be rejected"; exit 1; \
	else \
		echo "Duplicate appended fields rejected as expected"; \
	fi
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
