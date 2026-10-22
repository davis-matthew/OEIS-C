/*
 * A383249  Positive integers ending with the digit 1, or containing an odd
 *          digit d immediately followed by a digit >= d.
 * https://oeis.org/A383249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 147, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A383249: 
   a(n) is the nth positive integer that ends in digit 1, 
   or contains an odd digit d immediately followed by a digit >= d
   when read from left to right (i.e. d is followed by the next less
   significant digit).  A brute‑force generation is used.  */
#include <stdlib.h>
#include <stdbool.h>

/* Helper that returns true if x satisfies the defining condition. */
static bool good(idx_t x)
{
    /* 1. Ends with 1. */
    if ((x % 10) == 1)
        return true;

    /* 2. Check for an odd digit followed by a digit >= it. */
    idx_t tmp = x;
    idx_t digits[20];
    int len = 0;
    while (tmp > 0) {
        digits[len++] = tmp % 10;
        tmp /= 10;
    }
    /* digits[0] is least significant, digits[len‑1] most significant. */
    for (int i = len - 1; i >= 1; --i) {
        idx_t left = digits[i];
        if ((left & 1) /* odd */) {
            idx_t right = digits[i - 1];
            if (right >= left)
                return true;
        }
    }
    return false;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined offset, harmless */

    /* Static cache of found terms. */
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* allocated capacity */
    static idx_t count = 0;  /* number of cached terms */
    static idx_t probe = 1;  /* next integer to test */

    /* Grow cache if necessary. */
    if (cache == NULL) {
        cap = 8;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache)
            abort();
    }

    /* Generate terms until we have at least n terms cached. */
    while (count < n) {
        if (good(probe)) {
            if (count == cap) {
                /* Double capacity. */
                size_t newcap = cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp)
                    abort();
                cache = tmp;
                cap = newcap;
            }
            cache[count++] = (seq_t)probe;
        }
        ++probe;
    }

    return cache[n - 1];
}
