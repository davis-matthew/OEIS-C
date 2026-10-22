/*
 * A385463  Consecutive internal states of the linear congruential
 *          pseudo-random number generator (7141*s + 54773) mod 259200 when
 *          started at 1.
 * https://oeis.org/A385463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A385463: a(n) = (7141*a(n-1)+54773) mod 259200, a(1)=1 */
/* This function memoises terms so that calls with arbitrary n (non‑monotonic) are fast. */
#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    static seq_t *cache  = NULL;   /* array of computed terms, cache[0] = a(1) */
    static idx_t  len    = 0;      /* number of terms currently stored         */
    static idx_t  cap    = 0;      /* allocated capacity of cache array         */

    /* sequence defined only for n >= 1 */
    if (n == 0)
        return (seq_t)0;

    /* constants cast to seq_t – all operations use seq_t arithmetic */
    const seq_t MOD = (seq_t)259200ULL;
    const seq_t MUL = (seq_t)7141ULL;
    const seq_t ADD = (seq_t)54773ULL;

    /* initialise cache with the first term */
    if (len == 0) {
        if (cap == 0) {
            cap   = 1;
            cache = (seq_t *)malloc(cap * sizeof(seq_t));
            if (!cache)
                return (seq_t)0; /* allocation failure – return 0 as a sentinel */
        }
        cache[0] = (seq_t)1;  /* a(1) = 1 */
        len = 1;
    }

    /* return cached value if already available */
    if (n <= len)
        return cache[n - 1];

    /* grow cache to compute missing terms up to n */
    while (len < n) {
        /* resize if full */
        if (len == cap) {
            idx_t newcap = cap * 2;
            if (newcap == 0) newcap = 1;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp)
                return (seq_t)0; /* reallocation failure */
            cache = tmp;
            cap   = newcap;
        }

        /* recurrence: a(k) = (a(k-1)*7141 + 54773) mod 259200 */
        seq_t prev = cache[len - 1];
        cache[len] = (prev * MUL + ADD) % MOD;
        ++len;
    }

    return cache[n - 1];
}
