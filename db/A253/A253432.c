/*
 * A253432  Number of (n+1) X (5+1) 0..1 arrays with every 2 X 2 subblock
 *          diagonal minus antidiagonal sum nondecreasing horizontally,
 *          vertically and ne-to-sw antidiagonally.
 * https://oeis.org/A253432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*2^(n-1) + 156 for n>1, a(1) = 181 (empirical)
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)181;
    }
    /* For n ≥ 2 use the closed form 9·2^(n-1) + 156. */
    return (seq_t)9 * pow2(n - 1) + (seq_t)156;
}
