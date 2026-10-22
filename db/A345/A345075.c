/*
 * A345075  E.g.f.: exp( x*(1 + 2*x) / (1 - x - x^2) ).
 * https://oeis.org/A345075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=1, a(2)=7, a(3)=43, a(n) = (2*n-1)*a(n-1) +
 *            (n+2)*(n-1)*a(n-2) - (2*n-5)*(n-1)*(n-2)*a(n-3) -
 *            (n-1)*(n-2)*(n-3)*(n-4)*a(n-4) for n ≥ 4
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

/* Cache for memoised values */
static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

/* Ensure internal cache can hold indices up to n */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap) return;

    idx_t new_cap = (cache_cap == 0) ? 8 : cache_cap;
    while (new_cap <= n) new_cap <<= 1;

#ifdef __cplusplus
    /* For C++: use new/delete to allow non‑trivial types */
    seq_t *new_cache = new seq_t[new_cap];
    for (idx_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
    delete[] cache;
    cache = new_cache;
#else
    /* For C: use realloc */
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
#endif
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    if (n < cache_len) return cache[n];

    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)1;
        } else if (i == 2) {
            cache[i] = (seq_t)7;
        } else if (i == 3) {
            cache[i] = (seq_t)43;
        } else {
            seq_t a1 = cache[i-1];
            seq_t a2 = cache[i-2];
            seq_t a3 = cache[i-3];
            seq_t a4 = cache[i-4];

            seq_t coeff1 = (seq_t)((idx_t)2 * i - (idx_t)1);
            seq_t coeff2 = (seq_t)(((i + (idx_t)2) * (i - (idx_t)1)));
            seq_t coeff3 = (seq_t)(((idx_t)2 * i - (idx_t)5) * (i - (idx_t)1) * (i - (idx_t)2));
            seq_t coeff4 = (seq_t)((i - (idx_t)1) * (i - (idx_t)2) * (i - (idx_t)3) * (i - (idx_t)4));

            cache[i] = a1 * coeff1 + a2 * coeff2 - a3 * coeff3 - a4 * coeff4;
        }
    }
    cache_len = n + 1;
    return cache[n];
}
