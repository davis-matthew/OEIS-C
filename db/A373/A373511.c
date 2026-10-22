/*
 * A373511  Expansion of 1/(1 - x/(1 - 49*x^7)^(1/7)).
 * https://oeis.org/A373511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/7)} 49^k * binomial(n/7-1, k)
 *            Equivalent integer formula (avoids fractional binomials): a(n) =
 *            Sum_{k=0..floor(n/7)} 7^k * [Prod_{i=1..k} (n-7*i)] / k!
 * Range:     u8 n <= 15, u16 n <= 25, u32 n <= 37, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    idx_t m = n / 7;          /* floor(n/7) */
    seq_t sum   = 1;
    seq_t prod  = 1;          /* ∏(n-7*i) for i<k */
    seq_t fact  = 1;          /* k!          */
    seq_t pow7  = 1;          /* 7^k         */

    for (idx_t k = 1; k <= m; ++k) {
        prod *= (seq_t)(n - 7 * k);
        fact *= (seq_t)k;
        pow7 *= 7;

        seq_t term = pow7 * prod / fact;
        sum += term;
    }
    return sum;
}
