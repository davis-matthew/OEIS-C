/*
 * A357837  a(n) is the sum of the lengths of all the segments used to draw a
 *          square of side n representing a fishbone pattern using symmetric
 *          L-shaped tiles with side length 2.
 * https://oeis.org/A357837
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(ceiling(2*(n+1)^2/3) - 1)
 * Range:     u8 n <= 12, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to the sequence type */
    seq_t x = (seq_t)n + 1;          /* (n+1) */
    seq_t y = x * x;                /* (n+1)^2 */

    /* Compute ceil(2*y/3) without overflow:
       ceil(2*y/3) = 2*(y/3) + ceil( (y%3)*2 / 3 )  */
    seq_t m = (y / 3) * 2;          /* 2*(floor(y/3)) */
    seq_t mod = y % 3;
    if (mod)                        /* if y%3 == 1 or 2 */
    {
        m += ((mod * 2) + 2) / 3;   /* ceil((y%3)*2/3) */
    }

    /* a(n) = 2*(m - 1) */
    return 2 * (m - 1);
}
