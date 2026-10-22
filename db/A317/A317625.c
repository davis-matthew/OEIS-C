/*
 * A317625  a(n) = Sum_{k=1..n} phi(floor(n/k)) where phi is the Euler totient
 *          function.
 * https://oeis.org/A317625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \sum_{k=1}^{n} phi(\lfloor n/k\rfloor)
 * Range:     u8 n <= 78, u16 n <= 9718, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t i = 1;
    while (i <= n) {
        idx_t v     = n / i;            /* floor(n/k) */
        idx_t last  = n / v;            /* last k giving this v */
        seq_t cnt   = (seq_t)(last - i + 1);
        sum += calc_phi((seq_t)v) * cnt;
        i = last + 1;
    }
    return sum;
}
