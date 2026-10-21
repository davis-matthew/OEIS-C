/*
 * A266324  Decimal representation of the n-th iteration of the "Rule 19"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n = 0 * a(n) = 5 if n = 1 * a(n) = 0 if n is even
 *            and n > 0 * a(n) = 2^(2*n+1) - 1 if n is odd and n ≥ 3 * This
 *            implements the closed‑form derived from the OEIS entry for
 *            A266324
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    if (n == 1) {
        return (seq_t)5;
    }
    if ((n & (idx_t)1) == 0) {            /* even n > 0 */
        return (seq_t)0;
    }
    /* odd n ≥ 3 */
    idx_t exp = 2 * n + 1;                 /* exponent for 2^(2n+1) */
    seq_t val = pow2(exp);                 /* 2^(2n+1) */
    return val - (seq_t)1;                 /* subtract 1 */
}
