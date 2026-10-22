/*
 * A313739  Coordination sequence Gal.6.159.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) (n ≥ 1) = 5·n + 2·⌊n/12⌋ + [n mod 12 ≥ 6] + [n
 *            mod 12 ≥ 7]
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t sn = (seq_t)n;
    seq_t term = sn * 5;                      /* base 5·n           */
    term += 2 * (seq_t)(n / 12);              /* add 2 for every full 12‑block    */
    idx_t r = n % 12;                         /* remainder in the current block  */
    if (r >= 6) term += 1;                   /* add 1 for n mod 12 == 6 or 7    */
    if (r >= 7) term += 1;
    return term;
}
