/*
 * A319469  Number of partitions of n into exactly four nonzero decimal
 *          palindromes.
 * https://oeis.org/A319469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = number of partitions of n into exactly four nonzero
 *            decimal palindromes (computed via DP)
 * Range:     u8 n <= 387, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_palindrome.h"

/* Count partitions of n into exactly four non‑zero decimal palindromes.
   The algorithm builds all palindromes ≤ n and then uses a
   two‑dimensional DP that counts unordered combinations of exactly
   k = 4 parts.  */
seq_t a(idx_t n)
{
    /* Quick return for trivial cases */
    if (n < 4)
        return (seq_t)0;

    /* Generate all palindromes up to n */
    idx_t *pals = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    if (!pals) return (seq_t)0;
    idx_t pals_cnt = 0;

    for (idx_t v = 1; v <= n; ++v)
        if (is_palindrome((seq_t)v, 10))
            pals[pals_cnt++] = v;

    /* DP array: dp[k][s] stored in a linear buffer.
       k ranges 0..4, s ranges 0..n. */
    size_t row_size = (size_t)(n + 1);
    seq_t *dp = (seq_t *)malloc(5 * row_size * sizeof(seq_t));
    if (!dp) { free(pals); return (seq_t)0; }

    /* Initialize all to 0 */
    for (size_t i = 0; i < 5 * row_size; ++i)
        dp[i] = (seq_t)0;
    dp[0 * row_size + 0] = (seq_t)1;        /* base case: one way to sum 0 with 0 parts */

    /* DP: iterate over each palindrome as a coin value */
    for (idx_t idx = 0; idx < pals_cnt; ++idx) {
        idx_t p = pals[idx];
        for (idx_t s = p; s <= n; ++s) {
            for (idx_t k = 1; k <= 4; ++k) {
                dp[k * row_size + s] += dp[(k - 1) * row_size + (s - p)];
            }
        }
    }

    seq_t result = dp[4 * row_size + n];

    /* Clean up */
    free(dp);
    free(pals);

    return result;
}
