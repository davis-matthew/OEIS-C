/*
 * A313117  Coordination sequence Gal.6.134.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n≥1, a(n)=5*n − (2*B) if n≡0 (mod 12), otherwise
 *            a(n)=5*n − (2*B−1), where B = ⌊(n+11)/12⌋
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0U)
        return (seq_t)1;

    /* B = (n+11)/12, n%12 tells whether we are at the last
       position of the 12‑term block. */
    idx_t B = (n + 11) / 12;
    seq_t val = (seq_t)5 * (seq_t)n;

    if (n % 12 == 0U)
        val -= (seq_t)(2 * B);          /* subtract 2·B */
    else
        val -= (seq_t)(2 * B - 1);      /* subtract (2·B−1) */

    return val;
}
