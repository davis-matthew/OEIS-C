/*
 * A361932  G.f. A(x) satisfies A(x) = 1 + (x * A(x) / (1 - x))^3.
 * https://oeis.org/A361932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(n-1,n-3*k) *
 *            binomial(3*k,k) / (2*k+1)
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 24, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Handle the offset explicitly (a(0) = 1). */
    if (n == 0) {
        return 1;
    }

    idx_t maxk = n / 3;
    seq_t sum = 0;

    for (idx_t k = 0; k <= maxk; ++k) {
        /* C(n-1, n-3k) = binom(n-1, n-3k) */
        seq_t term1 = binom(n - 1, n - 3 * k);

        /* C(3k, k)                     */
        seq_t term2 = binom(3 * k, k);

        seq_t denom = (seq_t)(2 * k + 1);

        sum += (term1 * term2) / denom;
    }

    return sum;
}
