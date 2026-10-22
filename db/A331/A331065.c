/*
 * A331065  Coordination sequence for first kind of trivalent vertex in
 *          1-skeleton of pentagonal icositetrahedron.
 * https://oeis.org/A331065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 1 for n = 0 or 7, * a(n) = 3*min(n,7-n) for 1 ≤ n ≤ 6, *
 *            a(n) = 0 for n ≥ 8
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n >= 8)   return 0;
    if (n == 0 || n == 7) return 1;
    /* 1 ≤ n ≤ 6 */
    idx_t m = n < (7 - n) ? n : (7 - n);
    return (seq_t)3 * (seq_t)m;
}
