/*
 * A264409  a(n) = Sum_{k=0..n} binomial(n, k) * binomial((n-k)*k, k).
 * https://oeis.org/A264409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n, k) * binomial((n-k)*k, k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t Cnk = 1; /* binomial(n, 0) */

    for (idx_t k = 0; k <= n; ++k) {
        /* term = C(n, k) * C((n-k)*k, k) */
        seq_t c1 = Cnk;
        idx_t nk = (n - k) * k;
        seq_t c2 = binomial(nk, k);
        sum += c1 * c2;

        /* update Cnk for next k (C(n, k+1)) */
        if (k < n) {
            Cnk = (Cnk * (n - k)) / (k + 1);
        }
    }

    return sum;
}
