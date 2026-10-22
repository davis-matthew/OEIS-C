/*
 * A330931  Numbers k such that both k and k + 1 are Niven numbers in base 2
 *          (A049445).
 * https://oeis.org/A330931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 880, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A330931: k such that k and k+1 are Niven in base 2.
   The function builds the sequence by scanning integers and checking the
   required divisibility condition on the bit-sum.  The sequence is memoised
   so each term is generated only once even if a() is called with indices
   out of order. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Compute the sum of binary digits of x.  The result is small enough
   to fit in seq_t, and the loop terminates quickly for typical values. */
static inline seq_t bit_sum(seq_t x)
{
    seq_t sum = 0;
    while (x != 0) {
        sum += (x & 1);
        x >>= 1;
    }
    return sum;
}

/* Dynamic array used to cache the sequence values. */
static seq_t *cache       = NULL;   /* pointer to the array */
static size_t cache_cap   = 0;       /* allocated capacity */
static size_t cache_len   = 0;       /* number of terms stored */

/* The next candidate integer to test for membership in the sequence. */
static idx_t scan_start = 1;

/* Grow the cache array to at least 'needed' elements. */
static void grow_cache(size_t needed)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* unrecoverable error */
    cache = tmp;
    cache_cap = new_cap;
}

/* Ensure that the cache contains at least 'n' terms.  Generates terms
   on demand using a simple linear scan. */
static void ensure_n(idx_t n)
{
    if (cache == NULL) {        /* first call: initialise */
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
    }

    size_t target = (size_t)n;
    while (cache_len < target) {
        seq_t x  = (seq_t)scan_start;
        seq_t s  = bit_sum(x);
        if (s != 0 && (x % s) == 0) {
            seq_t y = x + 1;
            seq_t s2 = bit_sum(y);
            if (s2 != 0 && (y % s2) == 0) {
                if (cache_len >= cache_cap)
                    grow_cache(cache_len + 1);
                cache[cache_len++] = x;
            }
        }
        ++scan_start;
    }
}

/* Return the nth term of the sequence (n >= 1).  The index is 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0)          /* out of bounds; defined to return 0 */
        return (seq_t)0;
    ensure_n(n);
    return cache[(size_t)n - 1];
}
