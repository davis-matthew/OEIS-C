/*
 * A255381  Number of strings of k+n decimal digits that contain one string of
 *          exactly k consecutive "0" digits, where k >= n.
 * https://oeis.org/A255381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0)=1; a(1)=18; for n>=2, a(n)=(81*n+99)*10^(n-2)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    if (n == 1) {
        return (seq_t)18;
    }
    /* n >= 2 */
    seq_t term = (seq_t)81 * (seq_t)n + (seq_t)99;
    seq_t pow   = pow10(n - 2);
    return term * pow;
}
