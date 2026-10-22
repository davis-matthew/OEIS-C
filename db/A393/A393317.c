/*
 * A393317  Maximum halting time for any of the 4096 Wolfram 2-state 2-symbol
 *          Turing machines across all n-bit inputs.
 * https://oeis.org/A393317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   a(n) = 2*(n+2)^2 - 1 for n < 5, 2^(n+2) - 3 for n >= 5
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* the sequence is defined for n >= 1; for n < 1 return 0 (unused) */
    if (n < 1)
        return 0;

    if (n < 5) {
        seq_t m = (seq_t)(n + 2);
        seq_t sq = m * m;
        return 2 * sq - 1;
    } else {
        return pow2(n + 2) - 3;
    }
}
