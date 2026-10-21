/*
 * A290762  Number of minimal edge covers in the n-gear graph.
 * https://oeis.org/A290762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) – 14*a(n-2) + 19*a(n-3) – 21*a(n-4) + 18*a(n-5)
 *            – 11*a(n-6) + 7*a(n-7) – 2*a(n-8) + a(n-9) for n>9, with initial
 *            values: a(1)=2, a(2)=6, a(3)=15, a(4)=38, a(5)=90, a(6)=200,
 *            a(7)=434, a(8)=934, a(9)=1995 (offset 1) The function memoises
 *            computed values so that calls with any order...
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 56, u128 n <= 112,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;         /* 1‑based array: cache[i] = a(i)     */
static idx_t cache_size = 0;        /* largest n currently computed       */
static size_t cache_cap  = 0;       /* allocated capacity (>= cache_cap)  */

/* Ensure that cache can hold indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n < cache_cap) return;
    size_t new_cap = (cache_cap ? cache_cap * 2 : 16);
    while (new_cap <= (size_t)n) new_cap *= 2;
    /* realloc returns void* in C++ – cast to seq_t* */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);              /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined, but return 0 for safety */

    /* Expand storage if necessary */
    ensure_capacity(n);

    /* Initialise base values up to 9 if not already done */
    if (cache_size < 9)
    {
        static const seq_t base[9] = {
            2, 6, 15, 38, 90, 200, 434, 934, 1995
        };
        idx_t upto = (n < 9 ? n : 9);
        for (idx_t i = 1; i <= upto; ++i)
            cache[i] = base[i-1];
        if (upto > cache_size) cache_size = upto;
    }

    /* Compute terms beyond the current cache size up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i)
    {
        seq_t val = 0;
        val += cache[i-1] * 6;
        val -= cache[i-2] * 14;
        val += cache[i-3] * 19;
        val -= cache[i-4] * 21;
        val += cache[i-5] * 18;
        val -= cache[i-6] * 11;
        val += cache[i-7] * 7;
        val -= cache[i-8] * 2;
        val += cache[i-9];
        cache[i] = val;
    }

    if (n > cache_size) cache_size = n;
    return cache[n];
}
