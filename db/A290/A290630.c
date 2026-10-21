/*
 * A290630  a(n+1) = a(n) + (final digit of greatest prime < a(n)); a(1)=3.
 * https://oeis.org/A290630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=3, a(n+1)=a(n)+(last digit of greatest prime < a(n)) */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"

/* Cache that stores the computed terms of the sequence. */
static seq_t *cache = NULL;        /* array of terms */
static size_t cache_len = 0;       /* number of terms currently stored */
static size_t cache_cap = 0;       /* allocated capacity */

/* Return the greatest prime < x (x >= 3). */
static seq_t greatest_prime_lt(seq_t x)
{
    seq_t p = x - (seq_t)1;            /* start at the largest number < x */
    while (!is_prime_big(p)) {         /* search downward */
        p = p - (seq_t)1;
    }
    return p;
}

/* a(n) for n >= 1 (a(1)=3).  Returns 0 for n == 0. */
seq_t a(idx_t n)
{
    if (n == 0)                       /* out of bounds */
        return (seq_t)0;

    /* Grow the cache if needed. */
    if (cache_cap < (size_t)n) {
        seq_t *tmp = seq_realloc(cache, cache_cap, (size_t)n);
        cache = tmp;
        cache_cap = (size_t)n;
        if (cache == NULL)            /* allocation failed */
            return (seq_t)0;
    }

    /* Initialise the first value if the cache is empty. */
    if (cache_len == 0) {
        cache[0] = (seq_t)3;
        cache_len = 1;
    }

    /* Compute any missing terms up to and including a(n). */
    for (idx_t i = cache_len; i < n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t gp   = greatest_prime_lt(prev);
        seq_t d    = gp % (seq_t)10;   /* last decimal digit of gp */
        cache[i]   = prev + d;
    }

    cache_len = (size_t)n;
    return cache[n - 1];
}
