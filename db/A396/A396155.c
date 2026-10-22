/*
 * A396155  a(n) is the number of possible outcomes having a run of 6 or more
 *          sixes when a die is rolled n times.
 * https://oeis.org/A396155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 17, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A396155 – Number of sequences of length n of die rolls that contain a run of
 * 6 or more consecutive sixes.
 *
 * Formula implemented:
 *     Let b(n) be the number of length‑n sequences that avoid a run of ≥6 sixes.
 *     b(0)=1, b(k)=6^k for 1≤k≤5,
 *     for n≥6:  b(n) = 5·(b(n‑1)+…+b(n‑6)).
 *     Then a(n) = 6^n – b(n).
 *
 * The code keeps a memoised table of a(n) and b(n) and extends it as needed.
 * This supports calls in any order, runs in O(n) time per new value and uses only
 * standard C11 features, so it is valid under C++17 as well.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, free */

/* External linkage as required */
seq_t a(idx_t n)
{
    /* static cache state */
    static seq_t *a_cache = NULL;
    static seq_t *b_cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;      /* next index to compute */
    static seq_t power6 = 1;          /* 6^len for the current len */
    static seq_t sum6   = 1;          /* sum of the last six b values */

    /* initialise caches on first call */
    if (cache_len == 0)
    {
        cache_cap = 1;
        a_cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        b_cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!a_cache || !b_cache)
            return (seq_t)0;          /* out of memory – unlikely in tests */

        a_cache[0] = (seq_t)0;        /* a(0) = 0 */
        b_cache[0] = (seq_t)1;        /* b(0) = 1 (empty sequence) */
        cache_len  = 1;                /* next to compute */
    }

    /* grow arrays if needed */
    if ((size_t)n + 1 > cache_cap)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap <= (size_t)n + 1)
            new_cap <<= 1;

        seq_t *new_a = (seq_t *)realloc(a_cache, sizeof(seq_t) * new_cap);
        seq_t *new_b = (seq_t *)realloc(b_cache, sizeof(seq_t) * new_cap);
        if (!new_a || !new_b)
            return (seq_t)0;          /* out of memory */

        a_cache = new_a;
        b_cache = new_b;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    while (cache_len <= n)
    {
        idx_t len = cache_len;          /* current index to compute */

        /* advance power6 to 6^len */
        power6 = power6 * (seq_t)6;

        seq_t b_new;
        if (len < 6)
            b_new = power6;             /* all sequences are valid */
        else
            b_new = sum6 * (seq_t)5;    /* recurrence */

        seq_t a_new = power6 - b_new;

        a_cache[len] = a_new;
        b_cache[len] = b_new;

        /* update running sum of last six b values */
        sum6 += b_new;
        if (len >= 6)
            sum6 -= b_cache[len - 6];

        cache_len++;                    /* next index to compute */
    }

    return a_cache[n];
}
