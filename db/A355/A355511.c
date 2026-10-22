/*
 * A355511  a(n) is the number of monic polynomials of degree n over GF(11)
 *          without linear factors.
 * https://oeis.org/A355511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1 - z)^11/(1 - 11*z) - 1 a(n) = Σ_{k=0}^{min(n,11)}
 *            (-1)^k * C(11,k) * 11^{n-k} (n < 11) a(n) = 10^11 * 11^{n-11} (n
 *            ≥ 11)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    const idx_t P = 11;

    if (n >= P) {
        seq_t pow11 = big_pow((seq_t)11, (seq_t)(n - P));
        seq_t result = (seq_t)100000000000ULL;   /* 10^11 */
        return result * pow11;
    }

    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = big_pow((seq_t)11, (seq_t)(n - k));
        term = term * binomial((idx_t)11, k);
        if (k & 1)
            sum -= term;
        else
            sum += term;
    }
    return sum;
}
