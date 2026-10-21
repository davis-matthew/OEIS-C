/*
 * A266756  Nim value of subtraction game S(2,5,7) with a heap of n beans.
 * https://oeis.org/A266756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = pattern[n mod 22] where the pattern repeats
 * Range:     u8 n <= 131, u16 n <= 131, u32 n <= 131, u64 n <= 131,
 *            u128 n <= 131, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t period[22] = {
    0, 0, 1, 1, 0, 2, 1, 3, 2, 2,
    0, 3, 1, 0, 0, 1, 1, 2, 2, 3,
    3, 2
};

seq_t a(idx_t n)
{
    /* Use modulo 22 to get the index in the periodic pattern */
    idx_t r = n % 22;
    return period[r];
}
