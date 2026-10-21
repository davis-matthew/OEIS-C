/*
 * A284548  Lexicographically earliest sequence such that a(0) = 1 and a(n +
 *          k) != a(n - k) for all 0 < k <= floor(n/a(n)).
 * https://oeis.org/A284548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 584, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Recurrence:  a(0) = 1.
    For n > 0, a(n) is the smallest positive integer such that
    for every j with 0 ≤ j < n and n − j ≤ ⌊j / a(j)⌋, we have
        a(n) ≠ a(2·j − n).
    (No other restriction applies, since the definition only enforces
    a(j+k) ≠ a(j−k) for the *center* index j, not for the elements
    themselves.)  This implements the lexicographically earliest
    sequence A284548.  */

/*  The function below memoises the sequence in a static array.
    It is safe for calls in any order and only uses library
    facilities allowed by the problem statement.  */
#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache      = NULL;   /* a(i) values               */
static idx_t *fdiv       = NULL;   /* floor(i / a(i)) values    */
static size_t cache_cap  = 0;       /* allocated capacity of cache*/
static size_t cache_len  = 0;       /* number of entries stored (>=1) */

seq_t a(idx_t n)
{
    /* Initialise the cache on first call. */
    if (cache_len == 0) {
        cache_cap = 4;
        cache    = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        fdiv     = (idx_t *)malloc(cache_cap * sizeof(idx_t));
        cache[0] = (seq_t)1;
        fdiv[0]  = (idx_t)0;       /* floor(0 / 1) = 0 */
        cache_len = 1;
    }

    /* Expand the cache up to the requested index n. */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        /* Find the smallest candidate that satisfies the
           constraints coming from all earlier indices j < i. */
        seq_t cand = (seq_t)1;
        for (;;) {
            bool ok = true;

            /* Check constraints imposed by earlier positions j. */
            for (idx_t j = 0; j < i; ++j) {
                /* n−j must be ≤ floor(j / a(j)) to be relevant. */
                idx_t k = i - j;                 /* distance from j to i */
                if (k >= 1 && k <= fdiv[j]) {
                    /* The forbidden value comes from a[2*j−i]. */
                    idx_t l = 2 * j - i;         /* always non‑negative here */
                    if (cache[l] == cand) {
                        ok = false;
                        break;
                    }
                }
            }
            if (!ok) {          /* try next candidate */
                ++cand;
                continue;
            }

            /* No further checks are needed: a(i) itself is not constrained
               by the definition (the constraint only involves a(j±k)). */

            /* Accept candidate. */
            if (cache_len == cache_cap) {
                cache_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                fdiv  = (idx_t *)realloc(fdiv,  cache_cap * sizeof(idx_t));
            }
            cache[cache_len] = cand;
            /* Store floor(i / cand) for future use. */
            fdiv[cache_len] = (idx_t)((seq_t)i / cand);
            cache_len++;
            break;
        }
    }

    return cache[n];
}
