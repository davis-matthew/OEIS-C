/*
 * A314670  Coordination sequence Gal.5.50.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 4*n + * (n>=10)*(1 + (n-10)/10) + * (n>=11)*(1 +
 *            (n-11)/10) * where n is the index of the sequence (a(0)=1)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base term */
    seq_t result = (seq_t)1 + (seq_t)4 * (seq_t)n;

    /* Extra increments for indices 10+10t and 11+10t (t>=0) */
    if (n >= 10) {
        result += (seq_t)1 + (seq_t)( (n - 10) / 10 );
    }
    if (n >= 11) {
        result += (seq_t)1 + (seq_t)( (n - 11) / 10 );
    }

    return result;
}
