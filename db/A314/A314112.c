/*
 * A314112  Coordination sequence Gal.4.134.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n + floor((n+6)/8) + floor((n+1)/8), a(0) = 1
 * Range:     u8 n <= 48, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special-case the offset */
    if (n == 0) {
        return 1;
    }
    /* Compute the main expression.  All intermediate results fit in
       idx_t (uint64_t).  The final result is cast to seq_t. */
    seq_t res = (seq_t)5 * (seq_t)n;
    res += (seq_t)((n + 6) / 8);   /* count of n ≡ 2 (mod 8) in 1..n */
    res += (seq_t)((n + 1) / 8);   /* count of n ≡ 7 (mod 8) in 1..n */
    return res;
}
