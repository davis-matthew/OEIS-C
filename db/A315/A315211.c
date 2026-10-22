/*
 * A315211  Coordination sequence Gal.3.20.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315211
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>0, a(n) = 6*n - 2*( 2*floor((n-1)/4)+min(2,
 *            (n-1)%4) )
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* seq_t and idx_t are defined by the build (unsigned integer types). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                     /* a(0) */

    /* For n ≥ 1 use the derived closed‑form formula. */
    idx_t m = n - 1;                   /* terms after the first one */
    idx_t k = m / 4;                   /* number of full 4‑term blocks */
    idx_t r = m % 4;                   /* remaining terms (0..3) */
    idx_t f = 2 * k + (r < 2 ? r : 2); /* number of +4 contributions */

    /* Compute the final value using idx_t arithmetic, then cast to seq_t. */
    return (seq_t)(6 * n - 2 * f);
}
