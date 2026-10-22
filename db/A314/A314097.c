/*
 * A314097  Coordination sequence Gal.6.354.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coordination sequence Gal.6.354.6 (OEIS A314097).
 *            Computed piecewise from n modulo 6. For n = 0 the value is 1.
 *            For k = floor(n/6) and r = n % 6: r = 0 : a = 31*k (k ≥ 1,
 *            a(0)=1) r = 1 : a = 31*k + (k odd ? 6 : 5) r = 2 : a = 31*k + 11
 *            r = 3 : a = 31*k + (k odd ? 15 : 16) r = 4 : a = 31*k...
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t k = n / 6;
    idx_t r = n % 6;

    switch (r)
    {
        case 0:
            return (seq_t)31 * k;

        case 1:
            return (seq_t)31 * k + ((k & 1) ? (seq_t)6 : (seq_t)5);

        case 2:
            return (seq_t)31 * k + (seq_t)11;

        case 3:
            return (seq_t)31 * k + ((k & 1) ? (seq_t)15 : (seq_t)16);

        case 4:
            return (seq_t)31 * k + (seq_t)20;

        case 5:
            return (seq_t)31 * k + ((k & 1) ? (seq_t)26 : (seq_t)25);
    }

    /* unreachable, but satisfies compiler */
    return (seq_t)0;
}
