/*
 * A355130  E.g.f. A(x) satisfies A(x) = 1 + x * A(2 * (1 - exp(-x))).
 * https://oeis.org/A355130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * Σ_{k=0}^{n-1} (-1)^{n-k-1} * 2^k * Stirling2(n-1,k) *
 *            a(k) (a(0)=1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache   = NULL;   /* a(0..n)  */
static seq_t *pow2    = NULL;   /* 2^k      */
static seq_t *prevS   = NULL;   /* Stirling row S(n-1,k) */
static size_t cache_cap = 0;
static size_t pow2_cap  = 0;
static size_t stir_cap  = 0;
static idx_t max_comp   = 0;

/* ensure array has room for at least 'need' elements */
static void grow_array(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = *cap ? *cap * 2 : 1;
        while (new_cap < need) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (tmp) *arr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* ---------- initialisation ---------- */
    if (cache_cap == 0) {
        grow_array(&cache,   &cache_cap, 1);
        grow_array(&pow2,    &pow2_cap,  1);
        grow_array(&prevS,   &stir_cap,  1);
        cache[0]  = (seq_t)1;   /* a(0) = 1 */
        pow2[0]   = (seq_t)1;   /* 2^0 = 1 */
        prevS[0]  = (seq_t)1;   /* S(0,0) = 1 */
        max_comp  = 0;
    }

    /* ---------- ensure storage for requested n ---------- */
    grow_array(&cache,   &cache_cap, n + 1);
    grow_array(&pow2,    &pow2_cap,  n + 1);
    grow_array(&prevS,   &stir_cap,  n + 1);

    /* ---------- compute missing terms ---------- */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        seq_t pos = (seq_t)0, neg = (seq_t)0;
        for (idx_t k = 0; k < i; ++k) {
            seq_t term = prevS[k];
            term = term * cache[k];
            term = term * pow2[k];
            if (((i - k - 1) & 1u) == 0u)
                pos += term;
            else
                neg += term;
        }
        seq_t diff = (pos >= neg) ? (pos - neg) : (neg - pos);
        cache[i] = (seq_t)i * diff;          /* a(i) = i * |sum| */

        pow2[i] = pow2[i-1] * (seq_t)2;      /* 2^i */

        /* compute next Stirling row S(i,k) */
        seq_t *newS = (seq_t *)malloc((i + 1) * sizeof(seq_t));
        newS[0] = (seq_t)0;                   /* S(i,0) = 0 for i>0 */
        for (idx_t k = 1; k <= i; ++k) {
            seq_t t1 = prevS[k-1];
            seq_t t2 = (k <= i-1) ? (prevS[k] * (seq_t)k) : (seq_t)0;
            newS[k] = t1 + t2;
        }
        free(prevS);
        prevS = newS;
    }
    max_comp = n;
    return cache[n];
}
