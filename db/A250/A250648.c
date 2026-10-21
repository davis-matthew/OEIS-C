/*
 * A250648  Number of length 4+1 0..n arrays with the sum of the maximum of
 *          each adjacent pair multiplied by some arrangement of +-1 equal to
 *          zero.
 * https://oeis.org/A250648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 20, a(2) = 125, a(3) = 476, a(4) = 1293, a(5) = 2954,
 *            a(6) = 5901, a(7) = 10766, a(8) = 18305, a(9) = 29478, a(10) =
 *            45361, a(11) = 67364, a(12) = 96961, a(13) = 135976, a(14) =
 *            186445. For n ≥ 15 it satisfies the linear recurrence a(n) =
 *            a(n‑1) + 2a(n‑2) + a(n‑3) − 4a(n‑4) − 5a(n‑5) + 3a(...
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 122, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A250648: number of length‑5 arrays 0..n whose adjacent pair maxima
   can be signed + or – to sum to zero.  */
#include <stdlib.h>

/* ---------- Core function ---------- */
seq_t a(idx_t n)
{
    /* Initial 14 terms (1‑based indexing) */
    static const seq_t init[14] = {
        20ULL,      125ULL,     476ULL,     1293ULL,     2954ULL,
        5901ULL,    10766ULL,   18305ULL,   29478ULL,    45361ULL,
        67364ULL,   96961ULL,   135976ULL,  186445ULL
    };

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cached_n = 0;

    /* For the first 14 indices, return the hard‑coded values. */
    if (n <= 14)
        return init[n - 1];

    /* Ensure the cache is large enough to hold values up to 'n'. */
    if (cache == NULL)
    {
        /* Allocate space for at least 14 elements initially. */
        cache = (seq_t *)malloc(14 * sizeof(seq_t));
        if (!cache) return 0;          /* out of memory: return 0 (unlikely). */
        cached_n = 14;
        for (idx_t i = 1; i <= 14; ++i)
            cache[i - 1] = init[i - 1];
    }

    if (n > cached_n)
    {
        /* Reallocate enlarged array. */
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!tmp) return 0;            /* out of memory: return 0. */
        cache = tmp;

        /* Compute new terms from cached_n+1 up to n. */
        for (idx_t i = cached_n + 1; i <= n; ++i)
        {
            /* i > 14 by construction here. */
            /* Positive part of the recurrence. */
            seq_t pos =  cache[i - 2]                  /* a(i‑1) */
                       + (seq_t)2 * cache[i - 3]        /* 2*a(i‑2) */
                       +       cache[i - 4]              /* a(i‑3) */
                       + (seq_t)3 * cache[i - 7]        /* 3*a(i‑6) */
                       + (seq_t)6 * cache[i - 8]        /* 6*a(i‑7) */
                       + (seq_t)3 * cache[i - 9]        /* 3*a(i‑8) */
                       +       cache[i -12]             /* a(i‑11) */
                       + (seq_t)2 * cache[i -13]        /* 2*a(i‑12) */
                       +       cache[i -14];            /* a(i‑13) */

            /* Negative part of the recurrence. */
            seq_t neg =  (seq_t)4 * cache[i - 5]        /* 4*a(i‑4) */
                       + (seq_t)5 * cache[i - 6]        /* 5*a(i‑5) */
                       + (seq_t)5 * cache[i -10]        /* 5*a(i‑9) */
                       + (seq_t)4 * cache[i -11]        /* 4*a(i‑10) */
                       +       cache[i -15];            /* a(i‑14) */

            cache[i - 1] = pos - neg;   /* pos  ≥  neg for all n≥15 */
        }
        cached_n = n;
    }

    return cache[n - 1];
}
