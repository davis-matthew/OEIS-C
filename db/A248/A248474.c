/*
 * A248474  Numbers congruent to 13 or 17 mod 30.
 * https://oeis.org/A248474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 30*floor((n-1)/2) + (13 if (n-1) is even else 17)
 * Range:     u8 n <= 17, u16 n <= 4369, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset starts at 1 */
    /* k = floor((n-1)/2) */
    seq_t k = (seq_t)(n - 1) / 2;
    seq_t val = k * (seq_t)30;
    val += ((n - 1) & 1) ? (seq_t)17 : (seq_t)13;
    return val;
}
