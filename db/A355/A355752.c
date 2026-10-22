/*
 * A355752  a(n) = 3*(2*n - 1)*(3*(2*n - 1)^3 + 1) / 2.
 * https://oeis.org/A355752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(2*n - 1)*(3*(2*n - 1)^3 + 1) / 2
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 87, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used; offset starts at 1 */
    seq_t k   = (seq_t)n * 2 - 1;   /* 2*n - 1 */
    seq_t k3  = k * k * k;          /* (2*n - 1)^3 */
    seq_t t   = k3 * 3 + 1;         /* 3*(2*n - 1)^3 + 1 */
    seq_t res = k * 3 * t / 2;      /* 3*(2*n - 1)*(t) / 2 */
    return res;
}
