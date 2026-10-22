/*
 * A337593  a(0) = 1; a(n) = (1/n) * Sum_{k=1..n} binomial(n,k)^2 * k *
 *          3^(k-1) * a(n-k).
 * https://oeis.org/A337593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * Sum_{k=1..n} binomial(n,k)^2 * k * 3^(k-1) *
 *            a(n-k) a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


/* Grow an array to at least (needed+1) elements. */
static void grow_cache(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap <= needed) {
        size_t new_cap = (*cap == 0) ? 1 : *cap;
        while (new_cap <= needed)
            new_cap <<= 1;                 /* double until large enough */
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* Compute a(n) with memoization. */
seq_t a(idx_t n)
{
    static seq_t *a_cache      = NULL;
    static size_t a_cap        = 0;
    static idx_t  a_last       = 0;        /* largest a computed so far */

    static seq_t *pow3_cache   = NULL;
    static size_t pow3_cap     = 0;
    static idx_t  pow3_last    = 0;        /* largest power of 3 computed */

    /* Ensure caches large enough for the requested n */
    grow_cache(&a_cache,   &a_cap, (size_t)n);
    grow_cache(&pow3_cache, &pow3_cap, (size_t)n);

    /* Base initialization */
    if (a_cap > 0 && a_last == 0)
        a_cache[0] = (seq_t)1;              /* a(0) = 1 */
    if (pow3_cap > 0 && pow3_last == 0)
        pow3_cache[0] = (seq_t)1;           /* 3^0 = 1 */

    /* Extend powers of 3 up to n‑1 (if needed) */
    if (n > 0) {
        for (idx_t i = pow3_last + 1; i <= n - 1; ++i)
            pow3_cache[i] = pow3_cache[i - 1] * (seq_t)3;
        pow3_last = n - 1;
    }

    /* Compute a(i) for any missing indices up to n */
    for (idx_t i = a_last + 1; i <= n; ++i) {
        seq_t sum   = (seq_t)0;
        seq_t binom = (seq_t)1;            /* binomial(i,0) */

        for (idx_t k = 1; k <= i; ++k) {
            /* binom(i,k) = binom(i,k-1)*(i-k+1)/k */
            binom = binom * (seq_t)(i - k + 1) / (seq_t)k;

            /* term = binom^2 * k * 3^(k-1) * a(i-k) */
            seq_t term = binom;
            term *= binom;                 /* binom^2 */
            term *= (seq_t)k;              /* *k */
            term *= pow3_cache[k - 1];     /* *3^(k-1) */
            term *= a_cache[i - k];        /* *a(i-k) */

            sum += term;
        }
        a_cache[i] = sum / (seq_t)i;       /* exact division by i */
    }
    a_last = n;

    return a_cache[n];
}
