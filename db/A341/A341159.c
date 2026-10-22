/*
 * A341159  Number of partitions of n into 6 distinct nonzero decimal
 *          palindromes.
 * https://oeis.org/A341159
 *
 * Offset:    21    (a(21) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 184, u16 n <= 1055, u32 n <= 9666, u64 n <= 9666,
 *            u128 n <= 9666, big any n
 * Helpers:   is_decimal_pal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of partitions of n into 6 distinct nonzero decimal palindromes
   computed via dynamic programming.
   OEIS sequence: A341159
   a(n) = |{ {p1,…,p6} ⊂ P : p_i distinct, p_i>0, Σp_i = n }|   where
   P = {decimal palindromes}.
   The algorithm enumerates all palindromes ≤ n and counts combinations
   with 6 distinct elements.  Complexity: O(n·|P|) ≈ O(n²) for n up
   to 10⁴, which is well within the required time limit. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_decimal_pal.h"

/* --- Main sequence function */
seq_t a(idx_t n)
{
    /* a(n) is defined only for n >= 21; for smaller indices return 0. */
    if (n < 21) {
        return 0;
    }

    /* Allocate DP table: dp[k][s] = number of ways to form sum s
       using exactly k distinct palindromes among those processed so far.
       we store as a flat array for speed. */
    const size_t K = 7;                     /* 0..6 inclusive */
    const size_t stride = (size_t)(n + 1);   /* sums 0..n */
    seq_t *dp = (seq_t *)calloc(K * stride, sizeof(seq_t));
    if (!dp) {
        return 0;   /* allocation failure – unlikely for the problem size */
    }

    dp[0 * stride + 0] = 1;   /* base: 0 palindromes sum to 0 */

    /* Enumerate all palindromes <= n and update DP once per palindrome
       to guarantee distinctness. */
    for (uint64_t v = 1; v <= (uint64_t)n; ++v) {
        if (!is_decimal_pal(v)) {
            continue;
        }
        /* Update DP in reverse order of k to avoid reusing the same
           palindrome. */
        for (int k = 5; k >= 0; --k) {
            size_t base_k  = (size_t)k   * stride;
            size_t base_k1 = (size_t)(k + 1) * stride;
            for (size_t s = 0; s + v <= (size_t)n; ++s) {
                seq_t val = dp[base_k + s];
                if (val) {
                    dp[base_k1 + s + v] += val;
                }
            }
        }
    }

    /* Result: number of partitions into 6 palindromes summing to n */
    seq_t result = dp[6 * stride + n];

    free(dp);
    return result;
}
