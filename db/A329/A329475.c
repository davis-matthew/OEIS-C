/*
 * A329475  a(n) = Sum_{k=0..n} C(n,k)^2*T(k)*T(n-k), where T(k) = A002426(k)
 *          is the coefficient of x^k in the expansion of (x^2+x+1)^k.
 * https://oeis.org/A329475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} C(n,k)^2 * T(k) * T(n-k) where T(k) is the
 *            coefficient of x^k in (1 + x + x^2)^k and T(0)=T(1)=1, T(k) for
 *            k>=2 satisfies T(k) = ((2*k-1)*T(k-1) + (3*(k-1))*T(k-2)) / k
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Global cache for T(n) */
static seq_t *T_cache = NULL;
static idx_t T_cap = 0;    /* allocated capacity */
static idx_t T_len = 0;    /* number of values computed [0, T_len) */

/* Ensure T_cache holds values up to index n (inclusive) */
static void ensure_T(idx_t n)
{
    if (n < T_len) return;              /* already available */

    /* Expand capacity if needed */
    if (T_cap == 0) {
        T_cap = 16;
        T_cache = (seq_t*)malloc(T_cap * sizeof(seq_t));
        if (!T_cache) return;  /* let function fail gracefully */
    }
    while (T_cap <= n) T_cap <<= 1;
    T_cache = (seq_t*)realloc(T_cache, T_cap * sizeof(seq_t));

    /* Compute missing values */
    if (T_len == 0) {        /* first element */
        T_cache[0] = (seq_t)1;
        T_len = 1;
    }
    if (T_len == 1) {        /* second element */
        T_cache[1] = (seq_t)1;
        T_len = 2;
    }
    for (idx_t i = T_len; i <= n; ++i) {
        seq_t term1 = ((seq_t)(2*i - 1)) * T_cache[i-1];
        seq_t term2 = ((seq_t)(3*(i-1))) * T_cache[i-2];
        T_cache[i] = (term1 + term2) / (seq_t)i;
    }
    T_len = n + 1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    ensure_T(n);                /* make sure T_cache has T[0..n] */

    seq_t sum   = (seq_t)0;
    seq_t binom = (seq_t)1;     /* C(n,0) */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t part1 = binom * T_cache[k];
        seq_t part2 = binom * T_cache[n - k];
        seq_t term  = part1 * part2;
        sum += term;

        if (k < n) {
            binom = binom * (seq_t)(n - k) / (seq_t)(k + 1);
        }
    }
    return sum;
}
