/*
 * A259902  n*a(n+1) = (2*n^2+3n-1)*a(n)-(n^2-n-2)*a(n-1); a(0)=0, a(1)=1.
 * https://oeis.org/A259902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A259902
 *
 *  Recurrence:
 *      n * a(n+1) = (2*n^2 + 3*n - 1) * a(n) - (n^2 - n - 2) * a(n-1)
 *  Initial values:
 *      a(0) = 0,  a(1) = 1,  a(2) = 4
 *
 *  This implementation memoises the sequence in a static array.
 *  It uses only integer arithmetic as required by the specification.
 *  The code is valid C11 and C++17.
 */

#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache = NULL;        /* array storing computed terms */
static idx_t  cache_len = 0;       /* allocated capacity of cache   */
static idx_t  max_computed = 2;    /* highest index that has a value */

/* Ensure the cache can hold indices up to 'needed' (inclusive)  */
static void ensure_capacity(idx_t needed)
{
    if (cache_len > needed) return;
    idx_t new_len = cache_len ? cache_len : 1;
    while (new_len <= needed) new_len <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) abort();          /* out of memory */
    cache = tmp;
    cache_len = new_len;
}

/* Initialise the first three terms if this is the first call */
static void init_cache(void)
{
    if (cache) return;
    ensure_capacity(2);          /* need indices 0, 1, 2 */
    cache[0] = (seq_t)0;
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)4;
    max_computed = 2;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    init_cache();

    if (n == 0) return cache[0];
    if (n == 1) return cache[1];
    if (n == 2) return cache[2];

    ensure_capacity(n);

    if (max_computed < n)
    {
        for (idx_t i = max_computed + 1; i <= n; ++i)
        {
            /* In the recurrence, let k = i-1, so that we compute a(i) from a(i-1) and a(i-2) */
            idx_t k = i - 1;
            seq_t kk = (seq_t)k;

            /* factor1 = 2*k^2 + 3*k - 1   (always non‑negative) */
            seq_t factor1 = (seq_t)2 * kk * kk + (seq_t)3 * kk - (seq_t)1;

            /* factor2 = k^2 - k - 2   (non‑negative for k >= 2, i.e. i >= 3) */
            seq_t factor2 = kk * kk - kk - (seq_t)2;

            seq_t numer = factor1 * cache[i - 1] - factor2 * cache[i - 2];
            cache[i] = numer / kk;       /* exact division by (i-1) */
        }
        max_computed = n;
    }
    return cache[n];
}
