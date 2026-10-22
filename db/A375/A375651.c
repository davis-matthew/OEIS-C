/*
 * A375651  Expansion of e.g.f. exp(x + x^2 * exp(x)).
 * https://oeis.org/A375651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/2)} (k+1)^(n-2k) / (k! * (n-2k)!)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* T holds n!/(k!*(n-2k)!) for current k.
       Initially k = 0, so T = 1. */
    seq_t T = 1;
    idx_t maxk = n / 2;          /* floor(n/2) */

    for (idx_t k = 0; k <= maxk; ++k)
    {
        idx_t exponent = n - 2 * k;
        seq_t base = (k + 1);
        seq_t powv = seq_pow(base, exponent);

        seq_t term = T * powv;
        sum += term;

        if (k == maxk) break;   /* no more k to process */

        /* Transition T_{k+1} = T_k * (n-2k-1)*(n-2k) / (k+1) */
        seq_t mul1 = (n - 2 * k - 1);
        seq_t mul2 = (n - 2 * k);
        T *= mul1;
        T *= mul2;
        T /= (k + 1);
    }
    return sum;
}
