/*
 * A386125  Primes having only {1, 4, 6, 9} as digits.
 * https://oeis.org/A386125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth prime whose decimal digits are from {1,4,6,9}. The
 *            function generates numbers using only these digits in increasing
 *            order and tests each for primality until it accumulates the
 *            requested number of primes
 * Range:     u8 n <= 7, u16 n <= 132, u32 n <= 48, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/grow_seq.h"
#include "../utils/is_prime_big.h"

static const int DIGITS[4] = {1, 4, 6, 9};

/* Generator state that yields all numbers consisting only of DIGITS in
   ascending numeric order. */
typedef struct {
    int len;               /* current number of digits */
    uint8_t idx[20];       /* indices into DIGITS for each position */
} gen_t;

static gen_t generator = { .len = 1,
                           .idx = {0} };

static uint64_t next_allowed_number(void) {
    /* Build the current number from the digit indices. */
    uint64_t val = 0;
    for (int i = 0; i < generator.len; ++i) {
        val = val * 10 + DIGITS[generator.idx[i]];
    }

    /* Increment the mixed‑radix counter for the next call. */
    int pos = generator.len - 1;
    while (pos >= 0) {
        if (++generator.idx[pos] < 4) {
            break;
        }
        generator.idx[pos] = 0;
        pos--;
    }
    if (pos < 0) {                 /* overflow: increase the number of digits */
        generator.len++;
        for (int i = 0; i < generator.len; ++i) {
            generator.idx[i] = 0;
        }
    }

    return val;
}

/* Dynamic array of primes found so far. */
static seq_t *prime_cache = NULL;
static size_t prime_cache_len = 0;
static size_t prime_cache_cap = 0;

/* Return the nth prime with digits in {1,4,6,9}. */
seq_t a(idx_t n) {
    if (n == 0) {
        return 0;   /* undefined for n < 1; harmless */
    }

    /* Grow the cache until it contains at least n primes. */
    while (prime_cache_len < (size_t)n) {
        uint64_t val = next_allowed_number();
        /* Only numbers ending in 1 or 9 can be prime (except 2) – filter out
           early to avoid unnecessary primality tests. */
        if ((val % 10 == 1) || (val % 10 == 9)) {
            if (is_prime_big((seq_t)val)) {
                grow_seq(&prime_cache, &prime_cache_cap, prime_cache_len + 1);
                prime_cache[prime_cache_len++] = (seq_t)val;
            }
        }
    }

    return prime_cache[n - 1];
}
