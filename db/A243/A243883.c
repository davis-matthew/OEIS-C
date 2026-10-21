/*
 * A243883  Numerator of circle radius r(n) at constant unit length sagitta
 *          and chord length = n.
 * https://oeis.org/A243883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator(n^2/8 + 1/2) * Simplified: a(n) =
 *            (n^2+4)/gcd(n^2+4,8)
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)n * (seq_t)n + (seq_t)4U;
    /* n even? */
    if ((n & 1U) == 0U) {
        /* n divisible by 4? */
        if ((n & 3U) == 0U)
            return val / (seq_t)4U;
        else    /* n%4==2 */
            return val / (seq_t)8U;
    }
    /* n odd */
    return val;
}
