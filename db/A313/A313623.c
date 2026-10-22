/*
 * A313623  Coordination sequence Gal.5.87.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n ≥ 1, a(n) = 5*n plus a correction * that is -1
 *            when n ≡ 3 (mod 10) and +1 when n ≡ 7 (mod 10)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;           /* a(0) = 1 */
    seq_t val = (seq_t)n * (seq_t)5;
    idx_t r = n % 10;
    if (r == 3) return val - (seq_t)1;   /* subtract 1 at n ≡ 3 (mod 10) */
    if (r == 7) return val + (seq_t)1;   /* add 1 at n ≡ 7 (mod 10) */
    return val;                         /* otherwise, 5*n */
}
