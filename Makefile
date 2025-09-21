# Makefile
CC = gcc
CFLAGS +=  -std=gnu11 -O3 -ffast-math -Wall -Wextra -Wno-unused-function
LDFLAGS += -lgmp
.NOTPARALLEL:

# rest of your Makefile

# Optimization flag
OPT_LEVEL = $(DEFAULT_OPT_LEVEL)
DEFAULT_OPT_LEVEL = GENERIC

# Alternative primes
ifeq ($(ALT_PRIMES),ALT)
PRIMES_VAL=1
else
PRIMES_VAL=0
endif

# Basic source files
SRC = random/random.c arith.c bench.c



# Add arithmetic source files
ifeq ($(OPT_LEVEL),GENERIC)
SRC64 = $(SRC)  p64/generic/arith_generic.c
SRC128 = $(SRC) p128/generic/arith_generic.c
SRC192 = $(SRC) p192/generic/arith_generic.c
SRC256 = $(SRC) p256/generic/arith_generic.c
SRC512 = $(SRC) p512/generic/arith_generic.c
else ifeq ($(OPT_LEVEL),FAST)
SRC64 = $(SRC)  p64/arm64/arith_arm64.c   p64/arm64/arith_arm64.S
SRC128 = $(SRC) p128/arm64/arith_arm128.c p128/arm64/arith_arm128.S
SRC192 = $(SRC) p192/arm64/arith_arm192.c p192/arm64/arith_arm192.S
SRC256 = $(SRC) p256/arm64/arith_arm256.c p256/arm64/arith_arm256.S
SRC512 = $(SRC) p512/arm64/arith_arm512.c p512/arm64/arith_arm512.S
endif

.PHONY: all arith clean

all: arith

arith: arith64 arith128 arith192 arith256 arith512


arith64: arith_tests.c $(SRC64)
	$(CC) $(CFLAGS) $(LDFLAGS)  -DSEC_LEVEL=0 -DPRIMES=$(PRIMES_VAL) -o $@ $^

arith128: arith_tests.c $(SRC128)
	$(CC) $(CFLAGS) $(LDFLAGS)  -DSEC_LEVEL=1 -DPRIMES=$(PRIMES_VAL) -o $@ $^

arith192: arith_tests.c $(SRC192)
	$(CC) $(CFLAGS) $(LDFLAGS)  -DSEC_LEVEL=2 -DPRIMES=$(PRIMES_VAL) -o $@ $^

arith256: arith_tests.c $(SRC256)
	$(CC) $(CFLAGS) $(LDFLAGS)  -DSEC_LEVEL=3 -DPRIMES=$(PRIMES_VAL) -o $@ $^

arith512: arith_tests.c $(SRC512)
	$(CC) $(CFLAGS) $(LDFLAGS)  -DSEC_LEVEL=4 -DPRIMES=$(PRIMES_VAL) -o $@ $^


clean:
	rm -f arith64 arith128 arith192 arith256 arith512





