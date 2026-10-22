/*
 * A337746  Maximum number of bishops within an n X n chessboard, where each
 *          bishop has a path to an edge.
 * https://oeis.org/A337746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   a(n) = 1 if n = 1 a(n) = 4 if n = 2 a(n) = 8 if n = 3 a(n) =
 *            2*(k² + 3k - 4) if n = 2k and k ≥ 2 a(n) = floor((3n² + 2)/4) -
 *            4 if n is odd and n ≥ 9 a(n) = floor((3n² + 2)/4) otherwise (n =
 *            5 or 7)
 * Range:     u8 n <= 10, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)              /* guard: undefined for offset 0 */
        return 0;

    /* Small cases that fall outside the general formulas */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)8;

    if ((n & 1) == 0) {      /* even n = 2k, k ≥ 2 */
        idx_t k = n >> 1;                 /* k = n/2 */
        seq_t kk = (seq_t)k * (seq_t)k;   /* k² */
        return (seq_t)2 * kk              /* 2k² */
               + (seq_t)6 * k             /* +6k */
               - (seq_t)8;                /* -8 */
    } else {                /* odd n, n ≥ 5 */
        seq_t part = (seq_t)3 * (seq_t)n * (seq_t)n + (seq_t)2;   /* 3n²+2 */
        seq_t val  = part / (seq_t)4;                              /* floor((3n²+2)/4) */
        if (n >= 9) val -= (seq_t)4;                              /* subtract 4 for n ≥ 9 */
        return val;
    }
}
