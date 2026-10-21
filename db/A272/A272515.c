/*
 * A272515  Number of set partitions of [n] into three blocks with distinct
 *          sizes.
 * https://oeis.org/A272515
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{1 ≤ a < b < c , a+b+c=n} n! / (a!·b!·c!) The
 *            implementation enumerates all triples a<b<c and uses
 *            multiplicative binomial coefficients to compute the multinomial
 *            coefficient n!/(a!b!c!_without_factorial·). Complexity: O(n²)
 *            per call, which is fast enough for n ≤ 10000
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 42, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 6)           /* offset 6 – values for n < 6 are not defined   */
        return 0;

    seq_t result = 0;

    /* bin_a_prev holds C(n, a−1) for the current iteration of a   */
    seq_t bin_a_prev = 1;   /* C(n,0) = 1                           */
    for (idx_t a = 1; a <= n - 3; ++a) {
        /* update bin_a to C(n, a) */
        bin_a_prev = bin_a_prev * (seq_t)(n - a + 1) / a;
        seq_t bin_a = bin_a_prev;

        idx_t remaining = n - a;                 /* n – a = b + c          */
        idx_t max_b = (remaining - 1) / 2;       /* ensure c > b            */

        seq_t bin_b = 1;                         /* C(remaining,0) = 1      */
        for (idx_t b = 1; b <= max_b; ++b) {
            /* C(remaining, b) from C(remaining, b-1) */
            bin_b = bin_b * (seq_t)(remaining - b + 1) / b;

            /* skip non‑distinct triples */
            if (b <= a)
                continue;

            /* c = n – a – b automatically > b because of max_b   */
            seq_t term = bin_a * bin_b;
            result += term;
        }
    }
    return result;
}
