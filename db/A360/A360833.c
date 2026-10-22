/*
 * A360833  Expansion of Sum_{k>=0} ( k * x / (1 - (k * x)^3) )^k.
 * https://oeis.org/A360833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} (n-3*k)^n * binomial(n-2*k-1, k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    idx_t maxk = n / 3;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t bin;
        if (k == 0) {
            bin = 1;
        } else {
            bin = binomial(n - 2 * k - 1, k);
        }

        seq_t power;
        idx_t m = n - 3 * k;
        if (m == 0 && n == 0) {
            power = 1;                   /* 0^0 treated as 1 */
        } else if (m == 0) {
            power = 0;                   /* positive exponent on 0 gives 0 */
        } else {
            power = big_pow((seq_t)m, (seq_t)n);
        }

        if (bin != 0) {
            sum += power * bin;
        }
    }
    return sum;
}
