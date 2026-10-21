/*
 * A253429  Number of (n+1) X (2+1) 0..1 arrays with every 2 X 2 subblock
 *          diagonal minus antidiagonal sum nondecreasing horizontally,
 *          vertically and ne-to-sw antidiagonally.
 * https://oeis.org/A253429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*2^(n-1) + 36 for n > 3; with initial values: a(1) = 39,
 *            a(2) = 58, a(3) = 73
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset is 1, value undefined for 0 */
    if (n == 1) return (seq_t)39;
    if (n == 2) return (seq_t)58;
    if (n == 3) return (seq_t)73;

    /* For n >= 4 use the closed form */
    seq_t val = pow2(n - 1);       /* 2^(n-1) in seq_t */
    val *= (seq_t)9;               /* multiply by 9 */
    val += (seq_t)36;              /* add 36 */
    return val;
}
