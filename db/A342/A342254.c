/*
 * A342254  Number of ways to write n as an ordered sum of ten powers of 2.
 * https://oeis.org/A342254
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to write n as an ordered sum of ten powers
 *            of 2
 * Range:     u8 n <= 13, u16 n <= 34, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Off‑set: a(10) is the first term (a(9) is undefined). */
/* Computation: repeated convolution of the sequence of powers of 2. */
/* Complexity: O(10 * n * log n) for n ≥ 10. */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, free */

seq_t a(idx_t n)
{
    /* a(10) is the first defined term; below offset the value is unspecified. */
    if (n < 10) return 0;

    /* Convert the index to a size_t for array indices; n is small (≤10 000). */
    size_t limit = (size_t)n;
    size_t N = limit + 1;

    /* Allocate two working arrays of seq_t. */
    seq_t *dp  = (seq_t *)malloc(N * sizeof(seq_t));
    seq_t *tmp = (seq_t *)malloc(N * sizeof(seq_t));
    if (!dp || !tmp) return 0;          /* memory allocation failed */

    /* Initialise dp for the zeroth part (empty sum). */
    for (size_t i = 0; i < N; ++i) dp[i] = 0;
    dp[0] = 1;

    /* Pre‑compute the powers of two that are ≤ n. */
    size_t pow[64];
    size_t cnt = 0;
    size_t p   = 1;
    while (p <= limit) {
        pow[cnt++] = p;
        if (p > limit / 2) break;      /* avoid overflow on next shift */
        p <<= 1;
    }

    /* Perform ten convolutions: each step adds one more power‑of‑two summand. */
    for (int part = 0; part < 10; ++part) {
        /* Initialise the temporary array to zero for this step. */
        for (size_t i = 0; i < N; ++i) tmp[i] = 0;

        /* Convolution with the generating function ∑ x^(2^k). */
        for (size_t pi = 0; pi < cnt; ++pi) {
            size_t power = pow[pi];
            for (size_t i = 0; i + power <= limit; ++i) {
                if (dp[i] != 0) tmp[i + power] += dp[i];
            }
        }

        /* Prepare for the next part: swap the roles of dp and tmp. */
        seq_t *swap = dp;
        dp = tmp;
        tmp = swap;
    }

    /* The result is the coefficient of x^n after 10 convolutions. */
    seq_t result = dp[limit];

    /* Clean up allocated memory. */
    free(dp);
    free(tmp);

    return result;
}
