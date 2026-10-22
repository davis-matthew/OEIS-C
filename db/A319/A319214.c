/*
 * A319214  a(n) = phi(n^5 - 1)/5 where phi is A000010.
 * https://oeis.org/A319214
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = phi(n^5 - 1)/5
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 120, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   calc_phi, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;
    seq_t nseq = (seq_t)n;
    seq_t n5   = seq_pow(nseq, 5);
    seq_t m    = n5 - 1;
    seq_t phi  = calc_phi(m);
    return phi / 5;
}
