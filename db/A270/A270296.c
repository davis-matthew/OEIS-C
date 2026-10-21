/*
 * A270296  Numbers which are representable as a sum of five but no fewer
 *          consecutive nonnegative integers.
 * https://oeis.org/A270296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*A001651(n) A001651(n) is the nth positive integer not
 *            divisible by 3 (1,2,4,5,7,8,10,11,13,14,...)
 * Range:     u8 n <= 8, u16 n <= 2184, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n >= 1 is assumed */
    seq_t ns = (seq_t)n;
    seq_t x;

    if ((ns & (seq_t)1) == (seq_t)1)   /* n odd */
        x = (3 * ns - 1) / 2;
    else                               /* n even */
        x = (3 * ns - 2) / 2;

    return x * 20;
}
