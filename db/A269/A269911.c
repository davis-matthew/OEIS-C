/*
 * A269911  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 3", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A269911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(0) = 1; a(n) = 4^(n+1) – 2^(n+2) – 3 for n > 0
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;
    seq_t pow4 = big_pow((seq_t)4, (seq_t)(n + 1));
    seq_t pow2 = big_pow((seq_t)2, (seq_t)(n + 2));
    return pow4 - pow2 - (seq_t)3;
}
