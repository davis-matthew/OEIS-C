/*
 * A275065  Number of permutations p of [n] such that p(i)-i is a multiple of
 *          ten for all i in [n].
 * https://oeis.org/A275065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \prod_{i=0}^{9} \lfloor (n+i)/10 \rfloor !
 * Range:     u8 n <= 17, u16 n <= 23, u32 n <= 33, u64 n <= 47, u128 n <= 71,
 *            big any n
 * Helpers:   factorial_u
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial_u.h"

seq_t a(idx_t n)
{
    idx_t m0  = n / 10;
    idx_t rem = n % 10;

    seq_t fact_m0   = factorial_u(m0);
    seq_t fact_m0p1 = factorial_u(m0 + 1);

    seq_t res = 1;
    for (idx_t i = 0; i < 10 - rem; ++i)
        res *= fact_m0;
    for (idx_t i = 0; i < rem; ++i)
        res *= fact_m0p1;

    return res;
}
