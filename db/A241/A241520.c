/*
 * A241520  Numbers k such that k^2 == -1 (mod 89).
 * https://oeis.org/A241520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -34*(-1)^n + 89*floor(n/2) (offset 1)
 * Range:     u8 n <= 6, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1-based */
    seq_t half = (seq_t)(n / 2);          /* floor(n/2) */
    seq_t val  = 89 * half;

    /* (-1)^n : 1 for even n, -1 for odd n */
    if (n & 1U) {                        /* n is odd */
        val += 34;                       /* -34*(-1) = +34 */
    } else {                             /* n is even */
        val -= 34;                       /* -34*(+1) = -34 */
    }
    return val;
}
