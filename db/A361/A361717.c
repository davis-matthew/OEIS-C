/*
 * A361717  a(n) = Sum_{k = 0..n-1} binomial(n-1,k)^2*binomial(n+k,k).
 * https://oeis.org/A361717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A361717
   a(n) = Sum_{k = 0..n-1} binomial(n-1,k)^2 * binomial(n+k,k)
*/
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    // Direct computation using the defining sum.
    seq_t binom1 = 1;   // binomial(n-1, 0)
    seq_t binom2 = 1;   // binomial(n+0, 0)
    seq_t sum = 0;

    for (idx_t k = 0; k < n; ++k) {
        // term = binom1^2 * binom2
        seq_t term = binom1 * binom1;
        term = term * binom2;
        sum = sum + term;

        // Update binom1 and binom2 for next k (if any)
        if (k + 1 < n) {
            idx_t num1 = n - 1 - k;
            idx_t den1 = k + 1;
            binom1 = (binom1 * (seq_t)num1) / (seq_t)den1;

            idx_t num2 = n + k + 1;
            idx_t den2 = k + 1;
            binom2 = (binom2 * (seq_t)num2) / (seq_t)den2;
        }
    }
    return sum;
}
