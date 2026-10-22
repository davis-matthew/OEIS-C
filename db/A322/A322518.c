/*
 * A322518  Binomial transform of the Apéry numbers (A005259).
 * https://oeis.org/A322518
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=0..n} binomial(n,k) * A(k) where A(k) =
 *            sum_{j=0..k} binomial(k,j)^2 * binomial(k+j,j)^2 (Apéry numbers)
 *            A(0) = 1, A(1) = 5
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for Apéry numbers */
static seq_t *A_cache = NULL;
static size_t A_cap = 0;

/* ensure cache has at least n+1 entries and compute missing Apéry numbers */
static void ensure_aperies(idx_t n)
{
    if (A_cap <= (size_t)n) {
        size_t new_cap = A_cap? A_cap * 2 : 16;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(A_cache, new_cap * sizeof(seq_t));
        if (!tmp) return; /* allocation failed; return and crash safely */
        A_cache = tmp;
        A_cap = new_cap;
    }
    /* Initialize first two Apéry numbers if they are missing */
    if (A_cap >= 2 && A_cache[0] == (seq_t)0 && A_cache[1] == (seq_t)0) {
        A_cache[0] = (seq_t)1;
        A_cache[1] = (seq_t)5;
    }
    /* Compute missing values */
    for (idx_t i = 0; i <= n; ++i) {
        if ((idx_t)i + 1 > (idx_t)A_cap) continue; /* safety */
        if ((idx_t)i < 2) continue; /* already initialized */
        if (A_cache[(size_t)i] != (seq_t)0) continue; /* already computed */
        seq_t sum = (seq_t)0;
        seq_t C1 = (seq_t)1;             /* binom(i,0)   */
        seq_t C2 = (seq_t)1;             /* binom(i+0,0) */
        seq_t B = (seq_t)1;              /* product at k=0 */
        sum += B * B;
        for (idx_t k = 1; k <= i; ++k) {
            /* update binomial factors */
            C1 = C1 * (seq_t)(i - (k - 1)) / (seq_t)k;          /* binom(i,k)   */
            C2 = C2 * (seq_t)(i + k) / (seq_t)k;                /* binom(i+k,k) */
            B = C1 * C2;                                       /* product at k  */
            sum += B * B;
        }
        A_cache[(size_t)i] = sum;
    }
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;
    /* Ensure Apéry numbers up to n are ready */
    ensure_aperies(n);
    /* Compute binomial transform */
    seq_t sum = (seq_t)0;
    seq_t binom = (seq_t)1; /* binom(n,0) */
    for (idx_t k = 0; k <= n; ++k) {
        sum += binom * A_cache[(size_t)k];
        if (k < n) {
            binom = binom * (seq_t)(n - k) / (seq_t)(k + 1); /* next binomial */
        }
    }
    return sum;
}
