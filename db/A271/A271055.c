/*
 * A271055  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 259", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A271055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(0)=1, a(1)=5, a(n)=4*4^n-4*2^n-12 for n>1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    seq_t pow4p = seq_pow((seq_t)4, n + 1);   /* 4^(n+1) */
    seq_t pow2p = seq_pow((seq_t)2, n + 2);   /* 2^(n+2) */
    seq_t result = pow4p - pow2p - (seq_t)12;
    return result;
}
