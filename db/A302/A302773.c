/*
 * A302773  Numerators of (3*n + 2)/12.
 * https://oeis.org/A302773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = (3*n + 2) / gcd(3*n + 2, 4) (i.e., the numerator of the
 *            fraction (3*n+2)/4 in lowest terms)
 * Range:     u8 n <= 84, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t num = (seq_t)n * 3 + 2;
    seq_t g   = 1;
    if ((num & 3) == 0)          /* divisible by 4 */
        g = 4;
    else if ((num & 1) == 0)     /* divisible by 2 */
        g = 2;
    return num / g;
}
