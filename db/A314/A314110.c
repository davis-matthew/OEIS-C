/*
 * A314110  Coordination sequence Gal.6.200.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n + ⌊(n+10)/12⌋ + ⌊(n+1)/12⌋ for n ≥ 2, a(0) = 1, a(1)
 *            = 5. This matches the OEIS sequence A314110
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    /* For n ≥ 2 compute the extra term contributed by the
       periodic pattern of increments at positions congruent to
       2 or 11 modulo 12.  The sum of the two floor divisions
       gives that count. */
    idx_t count = (n + 10) / 12 + (n + 1) / 12;
    return (seq_t)n * 5 + (seq_t)count;
}
