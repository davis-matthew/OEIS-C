/*
 * A271625  a(n) = = 2*(n+1)^2 - 5.
 * https://oeis.org/A271625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(n+1)^2 - 5
 * Range:     u8 n <= 10, u16 n <= 180, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1‑indexed; compute 2*(n+1)^2 - 5 */
    seq_t t   = (seq_t)n + 1;          /* t = n+1   */
    seq_t res = 2 * t * t - 5;         /* 2*(t^2) - 5 */
    return res;
}
