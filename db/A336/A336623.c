/*
 * A336623  First member of the Diophantine pair (m, k) that satisfies 8*(m^2
 *          + m) = k^2 + k; a(n) = m.
 * https://oeis.org/A336623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A336623:  a(n) = 34*a(n-2) - a(n-4) + 16  (n>=4)
   initial values: a(0)=0, a(1)=5, a(2)=11, a(3)=186
   The sequence is computed iteratively and cached so that
   arbitrary order of calls is handled efficiently. */
#include <stdlib.h>
#include <stdint.h>

static seq_t *cache       = NULL;   /* dynamically allocated array */
static size_t cache_cap   = 0;       /* current allocated capacity */
static size_t cache_len   = 0;       /* number of values currently stored (>=0) */

/* Ensure that the cache can hold indices up to n (inclusive). */
static void grow_cache(idx_t n)
{
    idx_t need = n + 1;
    if ((size_t)need <= cache_cap)
        return;

    size_t new_cap = cache_cap == 0 ? 16 : (size_t)need > 2*cache_cap ? (size_t)need : 2*cache_cap;
    seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
    if (!tmp) exit(1);          /* Out of memory – terminate the program. */
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute a(n) using the recurrence and cache the result. */
seq_t a(idx_t n)
{
    /* Allocate or grow the cache. */
    grow_cache(n);

    /* Supply missing initial values if necessary. */
    if (cache_len <= 0) {                      /* a(0) */
        cache[0] = 0;
        cache_len = 1;
    }
    if (cache_len <= 1) {                      /* a(1) */
        cache[1] = 5;
        cache_len = 2;
    }
    if (cache_len <= 2) {                      /* a(2) */
        cache[2] = 11;
        cache_len = 3;
    }
    if (cache_len <= 3) {                      /* a(3) */
        cache[3] = 186;
        cache_len = 4;
    }

    /* Compute missing entries up to n. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t val;
        if (i == 0)        val = 0;
        else if (i == 1)   val = 5;
        else if (i == 2)   val = 11;
        else if (i == 3)   val = 186;
        else {
            /* Recurrence: a(n) = 34*a(n-2) - a(n-4) + 16. */
            val = 34 * cache[i-2] - cache[i-4] + 16;
        }
        cache[i] = val;
    }
    cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}
