/*
 * A365779  Expansion of e.g.f. (exp(3*x) / (2 - exp(3*x)))^(5/6).
 * https://oeis.org/A365779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + Σ_{k=1}^{n-1} 3^k * binomial(n-1,k) * a(n-k),
 *            a(0)=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#ifdef __cplusplus
# include <new>
#endif

/* static cache for sequence values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* static cache for powers of 3 */
static seq_t *pow3 = NULL;
static size_t pow3_cap = 0;
static size_t pow3_len = 0;

/* ensure an array has at least 'needed' capacity, construct new elements if C++ */
static void ensure_capacity(seq_t **arr, size_t *cap, size_t needed)
{
    if (needed <= *cap)
        return;
    seq_t *tmp = (seq_t *)realloc(*arr, needed * sizeof(seq_t));
    if (!tmp)
        return;  /* allocation failed – program will crash on next use */
#ifdef __cplusplus
    for (size_t i = *cap; i < needed; ++i)
        ::new(&tmp[i]) seq_t(); /* default–construct new elements */
#endif
    *arr = tmp;
    *cap = needed;
}

/* grow the power‑of‑3 table up to index n (inclusive) */
static void ensure_pow3(idx_t n)
{
    if ((size_t)n + 1 <= pow3_len)
        return;
    ensure_capacity(&pow3, &pow3_cap, (size_t)n + 1);
    for (size_t i = pow3_len; i <= (size_t)n; ++i) {
        if (i == 0)
            pow3[0] = (seq_t)1;
        else
            pow3[i] = pow3[i-1] * (seq_t)3;
    }
    pow3_len = (size_t)n + 1;
}

/* grow the sequence cache up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    if ((size_t)n + 1 <= cache_len)
        return;
    ensure_capacity(&cache, &cache_cap, (size_t)n + 1);

    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i == 0) {                     /* base case a(0)=1 */
            cache[0] = (seq_t)1;
            continue;
        }
        /* a(i) = 5*a(i-1) + Σ_{k=1}^{i-1} 3^k * C(i-1,k) * a(i-k) */
        seq_t res = cache[i-1] * (seq_t)5;
        seq_t binom = (seq_t)1;
        for (idx_t k = 1; k < i; ++k) {
            binom = (binom * (seq_t)(i - k)) / (seq_t)k;   /* C(i-1,k) */
            seq_t term = pow3[k] * binom;
            term *= cache[i - k];
            res += term;
        }
        cache[i] = res;
    }
    cache_len = (size_t)n + 1;
}

/* sequence function */
seq_t a(idx_t n)
{
    ensure_pow3(n);
    ensure_cache(n);
    return cache[n];
}
