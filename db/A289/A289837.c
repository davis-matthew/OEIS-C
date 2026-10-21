/*
 * A289837  Number of cliques in the n-tetrahedral graph.
 * https://oeis.org/A289837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + binomial(n,3) + (pow2(n-2)-n+1)*binomial(n,2) +
 *            5*binomial(n,4)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 25, u64 n <= 55, u128 n <= 117,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/pow2.h"

/* Helper: binomial(n,2) */
static inline seq_t binom2(idx_t n)
{
    if (n < 2) return 0;
    return (seq_t)n * (seq_t)(n - 1) / 2;
}

/* Helper: binomial(n,3) */
static inline seq_t binom3(idx_t n)
{
    if (n < 3) return 0;
    return (seq_t)n * (seq_t)(n - 1) * (seq_t)(n - 2) / 6;
}

/* Helper: binomial(n,4) */
static inline seq_t binom4(idx_t n)
{
    if (n < 4) return 0;
    return (seq_t)n * (seq_t)(n - 1) * (seq_t)(n - 2) * (seq_t)(n - 3) / 24;
}

/* Sequence function */
seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For n==0 we return 0 for safety. */
    if (n == 0) return 0;
    if (n == 1) return 1;   /* base case */

    /* 2^(n-2) – valid for n >= 2 */
    seq_t pow2_val = pow2(n - 2);

    /* (2^(n-2) - n + 1) */
    seq_t term = pow2_val - (seq_t)n + 1;

    /* Compute the formula */
    seq_t result = 1;
    result += binom3(n);                     /* + C(n,3)     */
    result += term * binom2(n);              /* + (2n-2-n+1) * C(n,2) */
    result += 5 * binom4(n);                 /* + 5 * C(n,4) */

    return result;
}
