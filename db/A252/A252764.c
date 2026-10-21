/*
 * A252764  Number of length n primitive (=aperiodic or period n) n-ary words.
 * https://oeis.org/A252764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} n^d * μ(n/d) (OEIS A252764)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow, mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* not used, offset starts at 1 */
    seq_t result = 0;
    idx_t i;
    for (i = 1; (idx_t)(i * i) <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;
            seq_t m1 = (seq_t)mobius(n / d1);
            if (m1 != 0) {
                seq_t power = big_pow((seq_t)n, d1);
                result += power * m1;
            }
            if (d2 != d1) {
                seq_t m2 = (seq_t)mobius(n / d2);
                if (m2 != 0) {
                    seq_t power = big_pow((seq_t)n, d2);
                    result += power * m2;
                }
            }
        }
    }
    return result;
}
