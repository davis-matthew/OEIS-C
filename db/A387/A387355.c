/*
 * A387355  Upper (1/2)-midsequence of square numbers and triangular numbers;
 *          see Comments.
 * https://oeis.org/A387355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceiling((3*n^2 + n)/4)
 * Range:     u8 n <= 18, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a wider type for the intermediate calculation to avoid overflow.
       __uint128_t is available in GNU C/C++ compilers. */
    __uint128_t t   = n;               /* cast idx_t to 128-bit */
    __uint128_t num = 3 * t * t + t + 3; /* (3*n^2 + n + 3) for ceiling */
    return (seq_t)(num / 4);           /* floor division performs the ceiling */
}
