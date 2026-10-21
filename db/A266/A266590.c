/*
 * A266590  Decimal representation of the n-th iteration of the "Rule 37"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 2; for n > 1: if n is even: a(n) = 7 * 2^(n-1);
 *            if n is odd : a(n) = 2^(2n+1) - 31 * 2^(n-2) - 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use the provided power-of-two helper */
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)2;

    /* Even n > 1 */
    if ((n & 1u) == 0u) {
        /* 7 * 2^(n-1) */
        return (seq_t)7 * pow2(n - 1);
    }

    /* Odd n > 1 */
    /* 2^(2n+1) - 31 * 2^(n-2) - 1 */
    idx_t exp_high = 2 * n + 1;
    seq_t high = pow2(exp_high);
    seq_t mid  = (seq_t)31 * pow2(n - 2);
    seq_t res  = high - mid - (seq_t)1;
    return res;
}
