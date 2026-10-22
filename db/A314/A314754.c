/*
 * A314754  Coordination sequence Gal.5.82.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(23*n/5) + δ(n) where δ(0) = 1, and for n>0: let q =
 *            n/5, r = n%5. δ(n) = 1 if (r==1||r==4) && (q%2==0) or
 *            (r==2||r==3) && (q%2==1) else 0 The function implements this
 *            closed form
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;                 /* a(0) = 1 */

    idx_t q = n / 5;                     /* integer division */
    idx_t r = n % 5;

    /* base = floor(23*n/5) = 23*q + floor(23*r/5) */
    seq_t base = ((seq_t)23) * q + ((seq_t)23 * r) / 5;

    seq_t delta = 0;
    if ((r == 1 || r == 4) && ((q & 1) == 0))
        delta = 1;
    else if ((r == 2 || r == 3) && ((q & 1) == 1))
        delta = 1;

    return base + delta;
}
