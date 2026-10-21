/*
 * A290918  p-INVERT of the positive integers, where p(S) = (1 - S)^3.
 * https://oeis.org/A290918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 30*a(n-2) + 45*a(n-3) - 30*a(n-4) + 9*a(n-5) -
 *            a(n-6) Initial values: a(0) = 3, a(1) = 12, a(2) = 43, a(3) =
 *            147, a(4) = 486, a(5) = 1566
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 40, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Ensure cache has space for at least n+1 entries */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap)
        return;
    size_t new_cap = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* In the unlikely event of allocation failure, abort */
        abort();
    }
    /* Initialise new slots */
    for (size_t i = cache_cap; i < new_cap; ++i) {
        seq_t val;
        if (i == 0)       val = (seq_t)3;
        else if (i == 1)  val = (seq_t)12;
        else if (i == 2)  val = (seq_t)43;
        else if (i == 3)  val = (seq_t)147;
        else if (i == 4)  val = (seq_t)486;
        else if (i == 5)  val = (seq_t)1566;
        else {
            /* Recurrence implemented only with positive intermediate values */
            seq_t p = (seq_t)9 * new_cache[i-1]  /* 9*a(n-1) */
                    + (seq_t)45 * new_cache[i-3] /* 45*a(n-3) */
                    + (seq_t)9 * new_cache[i-5]; /* 9*a(n-5) */

            seq_t q = (seq_t)30 * new_cache[i-2] /* 30*a(n-2) */
                    + (seq_t)30 * new_cache[i-4] /* 30*a(n-4) */
                    +               new_cache[i-6];/* 1*a(n-6) */

            val = p - q; /* p ≥ q for n ≥ 6, so no underflow */
        }
        new_cache[i] = val;
    }
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
