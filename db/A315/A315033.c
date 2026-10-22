/*
 * A315033  Coordination sequence Gal.5.129.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For n > 0: Let k = n / 9 (integer division) Let r = n %
 *            9 If r == 0: a(n) = 44 * k else: a(n) = base[r] + 44 * k where
 *            base[0..8] = {1,5,9,15,19,25,29,35,39} This recurrence
 *            reproduces the OEIS sequence A315033 for all n ≥ 0
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base values for n = 0..8 */
    static const seq_t base[9] = {
        1, 5, 9, 15, 19, 25, 29, 35, 39
    };

    if (n == 0)
        return 1;            /* a(0) = 1 */

    /* compute quotient and remainder of division by 9 */
    idx_t k = n / 9;
    idx_t r = n % 9;

    if (r == 0)              /* multiples of 9: a(9k) = 44k */
        return (seq_t)44 * k;

    /* otherwise: a(n) = base[r] + 44*k */
    return base[r] + (seq_t)44 * k;
}
