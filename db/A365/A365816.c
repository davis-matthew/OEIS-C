/*
 * A365816  G.f. A(x) satisfies: A(x) = x * (1 + A(x))^3 / (1 - 3 * A(x)).
 * https://oeis.org/A365816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1 / n) * ∑_{k=0}^{n‑1} binomial(n+k‑1 , k) * *
 *            binomial(3*n , n‑k‑1) * 3^k (for n>0), * a(0) = 0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }

    /* Use an unsigned 128‑bit accumulator to avoid overflow of the
     * intermediate products when seq_t is uint64_t.  For arbitrary‑
     * precision builds seq_t is a big integer type and the cast at
     * the end preserves the correct result. */
    __uint128_t sum = 0;
    __uint128_t pow3 = 1;
    const idx_t three_n = 3 * n;

    for (idx_t k = 0; k < n; ++k) {
        seq_t c1 = binom(n + k - 1, k);         /* binomial(n+k‑1,k) */
        seq_t c2 = binom(three_n, n - k - 1);   /* binomial(3n,n‑k‑1) */

        __uint128_t term = (__uint128_t)c1 * (__uint128_t)c2;
        term = term * pow3;                     /* multiply by 3^k */
        sum += term;

        pow3 *= 3;                               /* advance 3^k */
    }

    /* Final division by n gives the integer value. */
    seq_t result = (seq_t)(sum / n);
    return result;
}
