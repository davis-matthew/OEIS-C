/*
 * A300075  Period 6: repeat [0, 1, 1, 2, 2, 2].
 * https://oeis.org/A300075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(((n+1) mod 6)/2) + 2*floor((n mod 6)/5)
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = (n + 1) % 6;   /* n+1 modulo 6 */
    idx_t s = n % 6;        /* n modulo 6 */

    idx_t term1 = r / 2;    /* floor(((n+1) mod 6)/2) */
    idx_t term2 = 2 * (s / 5); /* 2*floor((n mod 6)/5) */

    return (seq_t)(term1 + term2);
}
