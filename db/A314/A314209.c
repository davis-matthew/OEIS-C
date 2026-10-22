/*
 * A314209  Coordination sequence Gal.5.293.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = 5 + ⌊(n-1)/10⌋ * 54 + prefix[(n-1) mod 10] for
 *            n ≥ 1. The prefix array contains the cumulative sums of one
 *            period of the difference pattern [6,6,5,5,5,5,6,6,5,5]
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) */
    /* cumulative sums of the period length 10: 6,6,5,5,5,5,6,6,5,5 */
    static const seq_t prefix[11] = {
        (seq_t)0, (seq_t)6, (seq_t)12, (seq_t)17,
        (seq_t)22, (seq_t)27, (seq_t)32, (seq_t)38,
        (seq_t)44, (seq_t)49, (seq_t)54
    };
    idx_t m = n - 1;                 /* number of differences to add after a(1) */
    idx_t g = m / 10;                /* full periods */
    idx_t r = m % 10;                /* remaining partial period */
    return (seq_t)5 + (seq_t)g * (seq_t)54 + prefix[r];
}
