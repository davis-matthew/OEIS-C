/*
 * A340873  a(n) is the number of iterations of A245471 needed to reach 1
 *          starting from n.
 * https://oeis.org/A340873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(2*n) = a(n) + 1 Transformation A245471: if n is even,
 *            f(n)=n/2; if n is odd (>1), f(n)=2*(n ^ (n >> 1))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t steps = 0;
    while (n != 1) {
        if ((n & 1) == 0) {
            n >>= 1;                     /* even case */
        } else {
            idx_t x = n ^ (n >> 1);       /* n xor (n>>1) */
            n = x << 1;                   /* 2 * (n ^ (n>>1)) */
        }
        steps++;
    }
    return steps;
}
