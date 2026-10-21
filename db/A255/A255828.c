/*
 * A255828  a(n) = n for n < 8; a(8n) = a(n); if every 8th term (a(8), a(16),
 *          a(24),...) is deleted, this gives back the original sequence.
 * https://oeis.org/A255828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n/8) if n mod 8 == 0; otherwise a(n) = a(n -
 *            floor(n/8)), with a(n) = n for 1 ≤ n < 8
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 8)               /* initial segment */
        return (seq_t)n;

    idx_t m = n;
    /* iteratively reduce m until it falls below 8 */
    while (m >= 8) {
        if ((m & 7) == 0) {          /* m divisible by 8 -> a(m) = a(m/8) */
            m >>= 3;                 /* divide by 8 */
        } else {                     /* m not divisible by 8 -> a(m) = a(m - floor(m/8)) */
            m -= m >> 3;             /* subtract floor(m/8) */
        }
    }
    return (seq_t)m;          /* m is now < 8, thus a(m) = m */
}
