/*
 * A259665  a(0)=0, a(1)=1, a(n) = min{4 a(k) + (4^(n-k)-1)/3, k=0..(n-1)} for
 *          n>=2.
 * https://oeis.org/A259665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 29, u32 n <= 116, u64 n <= 471,
 *            u128 n <= 1934, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A259665:
 *   a(n) = sum_{i=0}^{n-1} 4^{A003056(i)}   (q = 4)
 *   a(0) = 0, a(1) = 1.
 * This implementation uses the block‑sum formula:
 *   a(n) = \sum_{k=0}^{K-1} (k+1)*4^k + (n - T_K)*4^K,
 * where K = floor( (sqrt(8*(n-1)+1)-1)/2 ) and T_K = K*(K+1)/2.
 *
 * The function is O(sqrt(n)) per call and works for any call order.
 * It relies only on the arithmetic operators defined for seq_t.
 */

#include <stdlib.h>
#include <math.h>

/* Helper: triangular number k*(k+1)/2 computed safely with 128‑bit arithmetic. */
static inline idx_t triangular(idx_t k)
{
    return (idx_t)(((__int128)k * (k + 1)) / 2);
}

/* Power of four cache (4^i). */
static seq_t   *pow4 = NULL;   /* array of 4^i */
static size_t  pow4_cap = 0;   /* allocated capacity for pow4 array */
static idx_t   pow4_len = 0;   /* highest index currently computed (pow4_len holds that value) */

/* Ensure pow4[0..up_to] are computed and cache is large enough. */
static void ensure_pow(idx_t up_to)
{
    if (pow4 == NULL) {
        pow4_cap = 4;
        pow4 = (seq_t *)malloc(pow4_cap * sizeof(seq_t));
        pow4[0] = 1;          /* 4^0 */
        pow4_len = 0;
    }
    if ((idx_t)pow4_len >= up_to) return;

    /* Grow the allocated array if necessary. */
    if ((size_t)up_to > pow4_cap - 1) {
        size_t new_cap = pow4_cap;
        while ((size_t)up_to > new_cap - 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(pow4, new_cap * sizeof(seq_t));
        pow4 = tmp;
        pow4_cap = new_cap;
    }

    /* Compute missing powers. */
    for (idx_t i = pow4_len + 1; i <= up_to; ++i) {
        pow4[i] = pow4[i - 1] * 4;
    }
    pow4_len = up_to;
}

/* Main function computing A259665. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    idx_t m = n - 1;

    /* Compute K = floor((sqrt(8m+1)-1)/2). */
    long double d = sqrtl(8.0L * (long double)m + 1.0L);
    idx_t K = (idx_t)((d - 1.0L) / 2.0L);

    /* Correct possible off‑by‑one errors due to floating rounding. */
    while (triangular(K) > m) --K;
    while (triangular(K + 1) <= m) ++K;

    idx_t T_K = triangular(K);

    /* Ensure we have 4^0 … 4^K. */
    ensure_pow(K);

    seq_t result = 0;
    for (idx_t k = 0; k < K; ++k) {
        seq_t term = pow4[k];
        term *= (k + 1);          /* multiply by small integer (k+1) */
        result += term;
    }
    result += pow4[K] * (n - T_K);

    return result;
}
