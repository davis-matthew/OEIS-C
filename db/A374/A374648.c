/*
 * A374648  a(n) = n! / (floor(n/3))! - n! / (floor(n/2))!.
 * https://oeis.org/A374648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! / (floor(n/3))! - n! / (floor(n/2))!
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* floor(n/3) and floor(n/2) as integer divisions */
    idx_t f3 = n / 3;
    idx_t f2 = n / 2;

    /* product from (floor(n/3)+1) to n:  n! / (floor(n/3))! */
    seq_t prod1 = 1;
    for (idx_t i = f3 + 1; i <= n; ++i)
        prod1 *= (seq_t)i;

    /* product from (floor(n/2)+1) to n:  n! / (floor(n/2))! */
    seq_t prod2 = 1;
    for (idx_t i = f2 + 1; i <= n; ++i)
        prod2 *= (seq_t)i;

    /* difference */
    return prod1 - prod2;
}
