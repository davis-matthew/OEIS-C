/*
 * A249498  Number of length 1+5 0..n arrays with every six consecutive terms
 *          having five times some element equal to the sum of the remaining
 *          five.
 * https://oeis.org/A249498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 81, u64 n <= 111, u128 n <= 111,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A249498
 *  Sequence: Number of length‑6 arrays with elements in {0..n}
 *  such that each block of six consecutive terms has an element x
 *  with 5x = sum of the other five terms.
 *
 *  Equivalent condition: for the entire array the sum S must be
 *  divisible by 6 and S/6 must appear among the six terms.
 *
 *  The routine computes a(n) by counting integer solutions
 *  with bounds via a sliding‑window DP on the generating function
 *  (1 + x + … + x^n)^6 and an inclusion–exclusion over the
 *  allowed value that must appear (the quotient S/6).
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* maximum possible sum for six numbers in [0..n] */
    idx_t maxSum = 6 * n;          /* < 2^63 for n <= 10^4 */
    size_t len = (size_t)maxSum + 1;   /* array length for DP tables */

    /* Allocate tables for dp[0..6][0..maxSum] */
    seq_t *dp[7];
    for (int m = 0; m <= 6; ++m) {
        dp[m] = (seq_t *)calloc(len, sizeof(seq_t));
        if (!dp[m]) { /* allocation failure – return 0 */
            for (int x = 0; x < m; ++x) free(dp[x]);
            return 0;
        }
    }

    /* dp[0][0] = 1 */
    dp[0][0] = (seq_t)1;

    /* Compute dp[m] = dp[m‑1] * (1 + x + … + x^n) for m = 1..6 */
    for (int m = 1; m <= 6; ++m) {
        seq_t *prev = dp[m - 1];
        seq_t *curr = dp[m];
        seq_t windowSum = 0;
        for (size_t i = 0; i < len; ++i) {
            windowSum += prev[i];
            if (i > (size_t)n) windowSum -= prev[i - (size_t)n - 1];
            curr[i] = windowSum;
        }
    }

    /* Binomial coefficients C(6,j) */
    const unsigned binom[7] = {1, 6, 15, 20, 15, 6, 1};

    seq_t answer = 0;

    /* For each possible quotient k (0 <= k <= n) */
    for (idx_t k = 0; k <= n; ++k) {
        idx_t S = 6 * k;
        seq_t total = dp[6][S];   /* number of sequences summing to 6k */

        /* Count sequences with zero occurrences of k, using inclusion–exclusion */
        seq_t pos = 0;   /* sum of terms with even j */
        seq_t neg = 0;   /* sum of terms with odd j */

        for (int j = 0; j <= 6; ++j) {
            idx_t shifted = S - (idx_t)j * k;
            if ((long long)shifted < 0) continue;      /* negative sum */
            if (shifted > maxSum) continue;            /* out of bounds */
            seq_t coeff = dp[6 - j][shifted];
            seq_t term  = coeff * (seq_t)binom[j];
            if ((j & 1) == 0) pos += term;
            else             neg += term;
        }
        seq_t zero = pos - neg;   /* sequences of length 6 with sum 6k and no k */

        seq_t count_k = total - zero;   /* sequences with at least one k */
        answer += count_k;
    }

    /* Free the DP tables */
    for (int m = 0; m <= 6; ++m) free(dp[m]);

    return answer;
}
