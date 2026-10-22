/*
 * A312475  Coordination sequence Gal.3.16.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Incremental definition:
   a(0) = 1
   a(1) = 4
   for n >= 2: a(n) = a(n-1) + 4 + ((n % 5) == 3 ? 2 : 0)  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    seq_t val = (seq_t)4;               /* value for a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        val += (seq_t)4;
        if ((i % 5) == 3)
            val += (seq_t)2;
    }
    return val;
}
