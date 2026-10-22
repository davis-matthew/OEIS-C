/*
 * A314249  Coordination sequence Gal.4.136.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*n - 1 - floor(n/7) - floor((n-1)/7)
 * Range:     u8 n <= 44, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 */

    /* n is >= 1 */
    seq_t n_seq   = (seq_t)n;
    seq_t part1   = n_seq / (seq_t)7;      /* floor(n/7)   */
    seq_t part2   = (n_seq - (seq_t)1) / (seq_t)7; /* floor((n-1)/7) */
    seq_t count5  = part1 + part2;           /* number of 5‑differences up to n */
    /* a(n) = 6*n - 1 - count5 */
    return (seq_t)6 * n_seq - (seq_t)1 - count5;
}
