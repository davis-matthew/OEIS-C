/*
 * A273331  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 654", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(0)=1, a(1)=5, a(2)=17, a(n) = (53*4^(n-2) - 5)/3 for n>2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)17;

    idx_t k   = n - 2;                /* exponent for 4^k */
    idx_t exp = (idx_t)(2 * k);       /* 4^k = 2^(2k) */

    seq_t p   = pow2(exp);            /* 4^k */
    seq_t val = p * (seq_t)53;       /* 53*4^(n-2) */
    seq_t res = (val - (seq_t)5) / (seq_t)3;
    return res;
}
