/*
 * A309335  a(n) = n^3 if n odd, 7*n^3/8 if n even.
 * https://oeis.org/A309335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n^3 if n odd; 7*n^3/8 if n even
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1626, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t v = (seq_t)n;
    if (n & 1U) {               /* odd indices */
        return v * v * v;       /* n^3 */
    } else {                    /* even indices */
        seq_t h = v >> 1;       /* n/2 */
        return 7 * h * h * h;   /* 7*(n/2)^3 = 7*n^3/8 */
    }
}
