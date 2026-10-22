/*
 * A300290  Period 6: repeat [0, 1, 2, 2, 3, 3].
 * https://oeis.org/A300290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n % 6 - floor((n % 6)/3) - floor((n % 6)/5)
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A300290: periodic with period 6: [0,1,2,2,3,3] */

seq_t a(idx_t n)
{
    /* precomputed values for n mod 6 */
    static const seq_t table[6] = {0, 1, 2, 2, 3, 3};
    return table[n % 6];
}
