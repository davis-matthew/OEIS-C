/*
 * A314671  Coordination sequence Gal.5.95.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n + 1 (for n < 9) a(n) = 4*n + 1 + 2*floor((n-9)/9) + 1
 *            + ((n-9) % 9 != 0) (for n ≥ 9)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = ((seq_t)4 * (seq_t)n) + (seq_t)1;
    if (n >= 9) {
        idx_t t = n - 9;                 /* n - 9, safe because n ≥ 9 */
        idx_t m = t / 9;                /* floor((n-9)/9) */
        idx_t r = t % 9;                /* (n-9) mod 9 */
        seq_t offset = ((seq_t)2 * (seq_t)m) + (seq_t)1;
        if (r != 0)
            offset += (seq_t)1;
        result += offset;
    }
    return result;
}
