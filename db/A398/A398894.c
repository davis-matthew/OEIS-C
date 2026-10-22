/*
 * A398894  Minimum value of k such that there exists a subset S of {1, ...,
 *          k} of size n for which the average of every nonempty subset of S
 *          is an integer.
 * https://oeis.org/A398894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n‑1)*lcm(1…n‑1) + 1 for n >= 3 a(1) = 1, a(2) = 3
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

static seq_t *L_cache = NULL;      /* lcm(1…i) for i = 0… */
static size_t L_cache_cap = 0;     /* allocated capacity of L_cache   */
static size_t L_cache_last = 0;    /* highest i that has been computed */

/* Ensure that L_cache is large enough and that lcm(1…m) is computed. */
static void ensure_lcm_cache(idx_t m)
{
    /* First allocation and initialization of L_cache[0] = 1 (lcm of empty set). */
    if (L_cache == NULL) {
        L_cache_cap = 1;
        L_cache = (seq_t *)malloc(L_cache_cap * sizeof(seq_t));
        if (!L_cache) exit(1);
        L_cache[0] = (seq_t)1;
        L_cache_last = 0;
    }

    /* Resize if necessary. */
    if ((size_t)m + 1 > L_cache_cap) {
        size_t new_cap = L_cache_cap ? L_cache_cap * 2 : 2;
        if (new_cap < (size_t)m + 1) new_cap = (size_t)m + 1;
        seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_arr) exit(1);
        for (size_t i = 0; i < L_cache_cap; ++i) new_arr[i] = L_cache[i];
        free(L_cache);
        L_cache = new_arr;
        L_cache_cap = new_cap;
    }

    /* Compute any missing lcm values up to m. */
    for (size_t i = L_cache_last + 1; i <= (size_t)m; ++i) {
        seq_t prev = L_cache[i - 1];
        seq_t cur  = (seq_t)i;
        seq_t g    = gcd(prev, cur);
        seq_t next = (prev / g) * cur;
        L_cache[i] = next;
    }
    L_cache_last = (size_t)m;
}

/* Return lcm(1…m). */
static inline seq_t lcm_upto(idx_t m)
{
    ensure_lcm_cache(m);
    return L_cache[(size_t)m];
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;

    idx_t m = n - 1;                     /* lcm(1…n‑1) is needed */
    seq_t L = lcm_upto(m);
    return ((seq_t)(n - 1)) * L + (seq_t)1;
}
