/*
 * A363864  a(n) = A143007(2*n,n).
 * https://oeis.org/A363864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0..n} binomial(2n,n-k)^2 * binomial(2n+k,k)^2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the provided binom(a,b) routine (see utils/binom.h). */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t two_n = 2 * n;
    for (idx_t k = 0; k <= n; ++k) {
        /* C(2n, n-k) and C(2n+k, k) */
        seq_t c1 = binom(two_n, n - k);
        seq_t c2 = binom(two_n + k, k);

        /* term = c1^2 * c2^2 */
        seq_t term = c1 * c1;
        term = term * (c2 * c2);

        sum += term;
    }
    return sum;
}
