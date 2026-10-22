/*
 * A349540  E.g.f.: exp(x) * (BesselI(0,6*x) + BesselI(1,6*x)).
 * https://oeis.org/A349540
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * binomial(k,floor(k/2)) * 3^k
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include <stdint.h> /* for uint64_t in idx_t, if needed */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t pow3 = 1;        /* 3^k, starts with k=0 */
    seq_t binom_nk = 1;    /* binomial(n,0) */

    for (idx_t k = 0; k <= n; ++k) {
        /* floor(k/2) */
        idx_t m = k >> 1;

        /* binomial(k, floor(k/2)) */
        seq_t binom_km = binomial(k, m);

        /* term = binom(n,k) * binom(k, floor(k/2)) * 3^k */
        seq_t term = binom_nk * binom_km;
        term = term * pow3;
        sum += term;

        /* next power of 3 */
        pow3 *= 3;

        /* advance binom(n,k) -> binom(n,k+1) if needed */
        if (k < n) {
            seq_t num = (seq_t)(n - k);   /* numerator (n-k) */
            seq_t den = (seq_t)(k + 1);   /* denominator (k+1) */
            binom_nk = binom_nk * num / den;
        }
    }

    return sum;
}
