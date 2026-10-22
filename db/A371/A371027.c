/*
 * A371027  a(n) = Sum_{k=1..n} binomial(n, k) * Pochhammer(1/4, k) * 4^k *
 *          a(n - k) for n > 0 and a(0) = 1. Row sums of triangle A371026.
 * https://oeis.org/A371027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(n,k) * Pochhammer(1/4,k) * 4^k *
 *            a(n-k), with a(0) = 1. The recurrence can be simplified to a(n)
 *            = Sum_{k=1..n} binomial(n,k) * Prod_{i=0}^{k-1}(4i+1) * a(n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Helper: resize a cache array to hold at least (n+1) elements. */
static void ensure_cache(idx_t n, seq_t **arr, size_t *cap)
{
    size_t need = (size_t)(n + 1);
    if (*cap < need) {
        size_t newcap = (*cap == 0) ? 4 : *cap * 2;
        while (newcap < need) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
        if (!tmp) exit(1);
        *arr = tmp;
        *cap = newcap;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Static caches */
    static seq_t *a_cache = NULL;   /* computed values a[0..] */
    static size_t a_len = 0;        /* number of values stored */
    static size_t a_cap = 0;        /* allocated capacity */

    static seq_t *p_cache = NULL;   /* table of P_k = ∏_{i=0}^{k-1}(4i+1) */
    static size_t p_len = 0;
    static size_t p_cap = 0;

    /* Ensure sufficient capacity for requested index */
    ensure_cache(n, &a_cache, &a_cap);
    ensure_cache(n, &p_cache, &p_cap);

    /* 1. Compute required P_k values up to n */
    while (p_len <= n) {
        if (p_len == 0) {
            p_cache[0] = 1;                /* empty product */
        } else {
            seq_t factor = (seq_t)(4 * (p_len - 1) + 1); /* 4i+1 where i=p_len-1 */
            p_cache[p_len] = p_cache[p_len - 1] * factor;
        }
        ++p_len;
    }

    /* 2. Compute a[n] (and any missing earlier values) */
    while (a_len <= n) {
        if (a_len == 0) {
            a_cache[0] = 1;                /* a(0) = 1 */
        } else {
            seq_t sum = 0;
            seq_t bin = 1;                 /* binomial current value */
            for (idx_t k = 1; k <= a_len; ++k) {
                /* Update bin: bin = bin * (n - k + 1) / k */
                bin = bin * ((seq_t)(a_len - k + 1)) / ((seq_t)k);
                seq_t term = bin * p_cache[k];
                term *= a_cache[a_len - k];
                sum += term;
            }
            a_cache[a_len] = sum;
        }
        ++a_len;
    }

    return a_cache[n];
}
