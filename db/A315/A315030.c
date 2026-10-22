/*
 * A315030  Coordination sequence Gal.6.344.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a_base[n % 12] + 58 * (n / 12) for n > 0; where a_base =
 *            {0,5,9,15,19,25,29,33,39,43,49,53} and a(0) = 1. The recurrence
 *            a(n+12) = a(n) + 58 holds for n≥1
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base values for the first 12 indices (with a_base[0] = 0). */
    static const seq_t base[12] = {0, 5, 9, 15, 19, 25, 29, 33, 39, 43, 49, 53};

    if (n == 0)
        return (seq_t)1;                     /* a(0) is 1 */

    idx_t q = n / 12;                         /* number of complete cycles */
    int r = (int)(n % 12);                    /* remainder 0..11 */

    return base[r] + (seq_t)58 * (seq_t)q;    /* a(n) = base + 58*q */
}
