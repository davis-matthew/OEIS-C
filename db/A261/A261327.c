/*
 * A261327  a(n) = (n^2 + 4) / 4^((n + 1) mod 2).
 * https://oeis.org/A261327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 + 4) / 4^((n+1) mod 2)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {                /* n even: a(2k) = 1 + k^2 */
        seq_t k = (seq_t)(n >> 1);
        return 1 + k * k;
    } else {                           /* n odd:  a(2k+1) = 5 + 4*k*(k+1) */
        seq_t k = (seq_t)(n >> 1);
        return 5 + 4 * k * (k + 1);
    }
}
