/*
 * A350233  Integers that are divisible by 5 and not by 4, and whose reverse
 *          is divisible by 4 and not by 5.
 * https://oeis.org/A350233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by testing consecutive multiples of 5 (starting at 5),
   keeping only those divisible by 5 but not by 4, whose reverse is
   divisible by 4 but not by 5. */
#include <stdlib.h>

/* Helper: reverse the decimal digits of x. */
static seq_t reverse_decimal(seq_t x) {
    seq_t rev = 0;
    while (x > 0) {
        rev = rev * 10 + (x % 10);
        x /= 10;
    }
    return rev;
}

/* Cached terms of the sequence. */
static seq_t *cache = NULL;      /* array of already found terms */
static size_t cache_size = 0;    /* number of terms stored in cache */
static size_t cache_cap = 0;     /* allocated capacity of cache */

/* The next integer (multiple of 5) to examine. */
static seq_t next_candidate = 5;

/* Find and return the next term satisfying the conditions. */
static seq_t next_term(void) {
    seq_t m = next_candidate;
    while (1) {
        /* Must be divisible by 5 and not by 4. */
        if ((m % 4) != 0) {
            seq_t rev = reverse_decimal(m);
            if ((rev % 4) == 0 && (rev % 5) != 0) {
                /* Prepare start for next search. */
                next_candidate = m + 5;
                return m;
            }
        }
        m += 5;   /* only multiples of 5 need to be considered */
    }
}

/* Return the nth term of the sequence (offset 1).  */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined for n<1, but harmless */

    /* Fill cache up to the requested index. */
    while (cache_size < (size_t)n) {
        seq_t val = next_term();

        /* Allocate or grow the cache as needed. */
        if (cache_cap == 0) {
            cache_cap = 4;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        } else if (cache_size == cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }

        cache[cache_size++] = val;
    }

    return cache[n - 1];
}
