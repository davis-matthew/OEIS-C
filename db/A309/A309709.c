/*
 * A309709  Number of binary digits that change when n is multiplied by 4.
 * https://oeis.org/A309709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = popcount(n XOR (n << 2))
 * Range:     u8 n <= 63, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count the number of binary digits that change when n is multiplied by 4 */
seq_t a(idx_t n)
{
    /* Align types and compute the XOR */
    seq_t x = (seq_t)n;
    seq_t y = (seq_t)( (seq_t)n << 2 );   /* 4 * n */
    x ^= y;

    /* Count set bits by repeatedly clearing the lowest set bit */
    seq_t cnt = 0;
    while (x != 0)
    {
        x &= x - 1;    /* clear lowest set bit */
        cnt++;
    }
    return cnt;
}
