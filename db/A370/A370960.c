/*
 * A370960  a(n) = number of acyclic orientations of the complete tripartite
 *          graph K_{2,2,n}.
 * https://oeis.org/A370960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * (3^n - 6 * 4^n + 12 * 5^n)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 53,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t exp = (seq_t)n;
    seq_t p3 = big_pow((seq_t)3, exp);
    seq_t p4 = big_pow((seq_t)4, exp);
    seq_t p5 = big_pow((seq_t)5, exp);
    seq_t inside = p3 - 6 * p4 + 12 * p5;
    return 2 * inside;
}
