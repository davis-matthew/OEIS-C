/*
 * A315267  Coordination sequence Gal.6.215.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=6, a(2)=10.
   For n≥3, a(n)=a(n-1)+p[(n-3)%9], where
   p = [5,5,8,5,5,4,6,6,4]. */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Periodic difference array for n ≥ 3 */
    static const seq_t pattern[9] = {
        (seq_t)5, (seq_t)5, (seq_t)8,
        (seq_t)5, (seq_t)5, (seq_t)4,
        (seq_t)6, (seq_t)6, (seq_t)4
    };

    seq_t val = (seq_t)1;          /* a(0)      */
    if (n >= 1) val += (seq_t)5;   /* diff for n=1 */
    if (n >= 2) val += (seq_t)4;   /* diff for n=2 */

    for (idx_t i = 3; i <= n; ++i) {
        idx_t idx = (i - 3) % 9;
        val += pattern[idx];
    }
    return val;
}
