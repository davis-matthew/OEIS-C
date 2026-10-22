/*
 * A315032  Coordination sequence Gal.4.72.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n == 0 * a(n) = 34 * (n/7) if n % 7 == 0 and n > 0 *
 *            a(n) = a(n % 7) + 34 * (n/7) otherwise * This follows from a(7k
 *            + r) = a(r) + 34k for r > 0 * and a(7k) = 34k for k >= 1
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base values for n = 0..6 */
    static const seq_t base[7] = {
        (seq_t)1, (seq_t)5, (seq_t)9, (seq_t)15,
        (seq_t)19, (seq_t)25, (seq_t)29
    };

    idx_t k = n / 7;          /* how many full blocks of 7 */
    idx_t r = n % 7;          /* position inside the block */

    if (r == 0) {             /* multiples of 7 */
        if (k == 0)           /* n == 0 case */
            return base[0];
        else
            return (seq_t)34 * (seq_t)k;
    } else {                  /* n % 7 != 0 */
        return base[r] + (seq_t)34 * (seq_t)k;
    }
}
