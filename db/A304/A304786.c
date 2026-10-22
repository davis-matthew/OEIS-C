/*
 * A304786  Expansion of Product_{k>=1} (1 - q(k)*x^k), where q(k) = number of
 *          partitions of k into distinct parts (A000009).
 * https://oeis.org/A304786
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥1} (1 - q(k) x^k), where q(k)
 *            = number of partitions of k into distinct parts
 * Range:     i8 n <= 25, i16 n <= 46, i32 n <= 51, i64 n <= 51, i128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *dp = NULL;          /* array for q(k) */
    static seq_t *b  = NULL;          /* array of product coefficients */
    static size_t  alloc = 0;         /* current allocation size */

    /* Ensure arrays are large enough. */
    if ((size_t)n + 1 > alloc) {
        alloc = (size_t)n + 1;
        dp = (seq_t *)realloc(dp, alloc * sizeof(seq_t));
        b  = (seq_t *)realloc(b,  alloc * sizeof(seq_t));
    }

    /* Initialize arrays. */
    const seq_t one = 1;
    const seq_t zero = 0;
    dp[0] = one;
    b[0]  = one;
    for (idx_t i = 1; i <= n; ++i) {
        dp[i] = zero;
        b[i]  = zero;
    }

    /* 1. Compute q(k) = number of partitions of k into distinct parts. */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = n; j >= i; --j) {
            dp[j] = dp[j] + dp[j - i];
        }
    }

    /* 2. Compute the coefficients of the product ∏_{k>=1}(1 - q(k) x^k). */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t ck = dp[k];               /* q(k) */
        for (idx_t j = n; j >= k; --j) {
            b[j] = b[j] - ck * b[j - k];
        }
    }

    return b[n];
}
