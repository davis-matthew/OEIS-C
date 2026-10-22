/*
 * A360775  Expansion of Sum_{k>=0} (x * (k + x^2))^k.
 * https://oeis.org/A360775
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} (n-2*k)^(n-3*k) * binomial(n-2*k,
 *            k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n / 3; ++k) {
        idx_t m = n - 2 * k;          /* n-2k */
        idx_t exp = n - 3 * k;        /* n-3k */
        seq_t power;
        if (m == 0 && exp == 0) {
            power = (seq_t)1;         /* convention: 0^0 = 1 */
        } else {
            power = big_pow((seq_t)m, (seq_t)exp);
        }
        seq_t bin = binomial(m, k);
        sum += power * bin;
    }
    return sum;
}
