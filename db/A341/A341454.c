/*
 * A341454  Number of partitions of n into 7 nonprime parts.
 * https://oeis.org/A341454
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into exactly 7 nonprime parts
 *            (nonprime meaning 1 or composite)
 * Range:     u8 n <= 47, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes ---------------------------------------------------------- */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Function ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Handle trivial cases: for n < 7 a(n) = 0, the DP will give 0 anyway */
    size_t Nplus = (size_t)n + 1;          /* n + 1 */

    /* ----------------------------------------------------------------
       Sieve of Eratosthenes: find all primes up to n
       ---------------------------------------------------------------- */
    bool *is_prime = (bool*)calloc(Nplus, sizeof(bool));
    if (!is_prime) return 0;               /* allocation failed */

    /* Mark all numbers >=2 as promising primes */
    for (idx_t i = 2; i <= n; ++i) is_prime[i] = true;

    /* Sieve */
    for (idx_t i = 2; i * i <= n; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= n; j += i) {
                is_prime[j] = false;
            }
        }
    }

    /* ----------------------------------------------------------------
       DP table: dp[j][s] -> number of ways to obtain sum s with j parts
       Use a flat array of length 8*(n+1)
       ---------------------------------------------------------------- */
    size_t total = 8 * Nplus;
    seq_t *dp = (seq_t*)calloc(total, sizeof(seq_t));
    if (!dp) {
        free(is_prime);
        return 0;
    }
    dp[0] = 1;                               /* dp[0][0] = 1 */

    /* ----------------------------------------------------------------
       Iterate over every allowed part value p (1 and composites)
       ---------------------------------------------------------------- */
    for (idx_t p = 1; p <= n; ++p) {
        if (!is_prime[p]) {                  /* p is nonprime */
            for (int j = 1; j <= 7; ++j) {
                size_t basej  = (size_t)j   * Nplus;
                size_t basejm = (size_t)(j-1) * Nplus;
                idx_t  remaining = (idx_t)(7 - j);      /* parts still needed after this one */

                idx_t max_s = n - remaining;           /* maximum sum reachable with j parts */
                if (max_s < p) continue;              /* impossible to use this part here */

                for (idx_t s = p; s <= max_s; ++s) {
                    dp[basej + s] += dp[basejm + (s - p)];
                }
            }
        }
    }

    /* Result: dp[7][n] */
    seq_t result = dp[7 * Nplus + n];

    /* Clean up */
    free(dp);
    free(is_prime);

    return result;
}
