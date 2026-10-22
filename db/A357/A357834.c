/*
 * A357834  a(n) = Sum_{k=0..floor(n/3)} Stirling1(n,3*k).
 * https://oeis.org/A357834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} Stirling1(n,3*k)
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 13, i64 n <= 20, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cache of already computed a(n) */
static size_t cache_cap = 0;         /* current capacity of the cache */
static size_t cache_len = 0;         /* number of stored values */
static seq_t A0 = 0, A1 = 0, A2 = 0; /* sums of s(n,k) for k≡0,1,2 (mod 3) */
static idx_t last_n = 0;             /* largest n that has been computed */

/* Grow the cache so that it can hold at least 'needed' elements. */
static void ensure_cache_cap(size_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        if (new_cap < needed) new_cap = needed;
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_ptr;
        cache_cap = new_cap;
    }
}

/* Returns a(n) for the sequence A357834. */
seq_t a(idx_t n)
{
    /* Initialise cache and state on the first call. */
    if (cache == NULL) {
        ensure_cache_cap(1);
        cache[0] = (seq_t)1;   /* a(0) = 1 */
        cache_len = 1;
        A0 = (seq_t)1;   /* sum for residue 0 at n = 0 */
        A1 = (seq_t)0;   /* residue 1 */
        A2 = (seq_t)0;   /* residue 2 */
        last_n = 0;
    }

    /* Return cached value if we already have it. */
    if (n < cache_len) {
        return cache[n];
    }

    /* Extend the cache up to n using the recurrence:
       A0(n) = A2(n‑1) - (n‑1)·A0(n‑1)
       A1(n) = A0(n‑1) - (n‑1)·A1(n‑1)
       A2(n) = A1(n‑1) - (n‑1)·A2(n‑1)    */
    for (idx_t i = last_n + 1; i <= n; ++i) {
        seq_t mult = (seq_t)(i - 1);
        seq_t nextA0 = A2 - mult * A0;
        seq_t nextA1 = A0 - mult * A1;
        seq_t nextA2 = A1 - mult * A2;

        A0 = nextA0;
        A1 = nextA1;
        A2 = nextA2;

        ensure_cache_cap(cache_len + 1);
        cache[cache_len] = A0;   /* a(i) is the sum for residue 0 */
        ++cache_len;
        last_n = i;
    }

    return cache[n];
}
