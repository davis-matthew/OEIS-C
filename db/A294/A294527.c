/*
 * A294527  Number of Dyck paths of length 2n with exactly 2 hills.
 * https://oeis.org/A294527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 21, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (OEIS A294527):
   a(0) = 0, a(1) = 0, a(2) = 1
   For n ≥ 3,
   a(n) = [(n-3)*(21n²-35n+10)*a(n-1) + 2*(n-1)*(2n-1)*(3n-2)*a(n-2)]
          /[2*(3n-5)*(n-2)*(n+1)]
   The denominator always divides the numerator exactly. */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;        /* cache[0..cache_capacity-1] holds computed values */
static size_t cache_size = 0;      /* number of entries currently stored (>=0) */
static size_t cache_capacity = 0;  /* allocated size of cache */

/* Ensure the cache can hold indices up to 'need' (inclusive).  */
static void ensure_capacity(idx_t need)
{
    if (need < cache_capacity)
        return;
    size_t new_cap = (cache_capacity > 0) ? cache_capacity * 2 : 16;
    while (new_cap <= (size_t)need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;
    cache_capacity = new_cap;
}

seq_t a(idx_t n)
{
    /* 0‑based offset: a(0) = 0.  */
    /* Handle small n immediately.  */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    /* Ensure cache can contain up to n.  */
    ensure_capacity(n);
    if (n < cache_size) return cache[n];

    /* Initialize base cases if not already done.  */
    if (cache_size == 0) {
        cache[0] = (seq_t)0;
        if (cache_capacity >= 2) cache[1] = (seq_t)0;
        if (cache_capacity >= 3) cache[2] = (seq_t)1;
        cache_size = 3;
    }

    /* Compute values from current cache_size up to n.  */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* Use 128‑bit arithmetic if available for intermediate products.  */
        seq_t ai_1 = cache[i-1];
        seq_t ai_2 = cache[i-2];

#if defined(__SIZEOF_INT128__)  /* GNU/Clang extensions */
        unsigned __int128 N1 = (unsigned __int128)(i - 3);
        N1 *= (unsigned __int128)(21 * i * i - 35 * i + 10);
        N1 *= (unsigned __int128)ai_1;

        unsigned __int128 N2 = (unsigned __int128)2 * (i - 1);
        N2 *= (unsigned __int128)(2 * i - 1);
        N2 *= (unsigned __int128)(3 * i - 2);
        N2 *= (unsigned __int128)ai_2;

        unsigned __int128 numerator   = N1 + N2;
        unsigned __int128 denominator = (unsigned __int128)2 * (3 * i - 5)
                                         * (i - 2)
                                         * (i + 1);
        cache[i] = (seq_t)(numerator / denominator);
#else
        /* Fallback: rely on seq_t big‑integer support (or 64‑bit if it fits).  */
        seq_t num1 = (seq_t)(i - 3) *
                     ((seq_t)21 * (seq_t)i * (seq_t)i - (seq_t)35 * (seq_t)i + (seq_t)10) *
                     ai_1;

        seq_t num2 = (seq_t)2 *
                     (seq_t)(i - 1) *
                     (seq_t)(2 * i - 1) *
                     (seq_t)(3 * i - 2) *
                     ai_2;

        seq_t numerator   = num1 + num2;
        seq_t denominator = (seq_t)2 *
                            (seq_t)(3 * i - 5) *
                            (seq_t)(i - 2) *
                            (seq_t)(i + 1);

        cache[i] = numerator / denominator;
#endif
    }
    cache_size = n + 1;
    return cache[n];
}
