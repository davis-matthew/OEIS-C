/*
 * A369687  a(n) = Sum_{p|n, p prime} p^phi(n/p).
 * https://oeis.org/A369687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{p|n, p prime} p^phi(n/p)
 * Range:     u8 n <= 20, u16 n <= 33, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   big_pow, calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/calc_phi.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    seq_t sum = 0;
    uint64_t tmp = n;
    uint64_t p;

    for (p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            uint64_t m = n / p;
            seq_t phi_m = calc_phi((seq_t)m);
            seq_t term   = big_pow((seq_t)p, phi_m);
            sum += term;

            while (tmp % p == 0)
                tmp /= p;
        }
    }

    if (tmp > 1)
    {
        uint64_t m = n / tmp;
        seq_t phi_m = calc_phi((seq_t)m);
        seq_t term   = big_pow((seq_t)tmp, phi_m);
        sum += term;
    }

    return sum;
}
