/*
 * A241521  Numbers k such that k^2 == -1 (mod 97).
 * https://oeis.org/A241521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -22*(-1)^n + 97*floor(n/2)
 * Range:     u8 n <= 5, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute floor(n/2) in seq_t arithmetic */
    seq_t half = (seq_t)(n / 2);
    seq_t result = half * 97;
    if ((n & 1) == 0)
        result -= 22;   /* n even */
    else
        result += 22;   /* n odd  */
    return result;
}
