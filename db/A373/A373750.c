/*
 * A373750  Middle terms of A373749.
 * https://oeis.org/A373750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   a(0)=0, a(1)=0, for n>=2, a(n)=⌊n/2⌋² mod n. Equivalent
 *            piecewise form: a(4k) = 0 a(4k+1) = 3k + 1 (k>=1) a(4k+2) = 2k +
 *            1 a(4k+3) = k + 1
 * Range:     u8 n <= 340, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    idx_t r = n & 3ULL;     /* n % 4, use bitwise for speed */
    switch (r) {
        case 0:                     /* n divisible by 4 */
            return (seq_t)0;
        case 1:                     /* n = 4k+1, k>=1 */
            return (seq_t)(3 * (n >> 2) + 1);
        case 2:                     /* n = 4k+2 */
            return (seq_t)(2 * (n >> 2) + 1);
        case 3:                     /* n = 4k+3 */
            return (seq_t)((n >> 2) + 1);
        default:                    /* unreachable, but keep for completeness */
            return (seq_t)0;
    }
}
