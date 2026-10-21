/*
 * A265888  a(n) = n + floor(n/4)*(-1)^(n mod 4).
 * https://oeis.org/A265888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(n/4)*(-1)^(n mod 4)
 * Range:     u8 n <= 205, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t k = (seq_t)(n / 4);
    idx_t r = n % 4;
    if ((r & 1ULL) == 0)           /* even r : (+1) */
        return (seq_t)n + k;
    else                           /* odd r : (-1) */
        return (seq_t)n - k;
}
