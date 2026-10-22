/*
 * A315612  Coordination sequence Gal.5.279.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Let k = n / 6 and r = n % 6. Then r = 0: a(0) = 1, a(6k) = 34 *
 *            k (k ≥ 1) r = 1: a(6k+1) = 34 * k + 6 r = 2: a(6k+2) = 34 * k +
 *            12 r = 3: a(3) = 14, a(6k+3) = 28 * k + 16 (k ≥ 1) r = 4:
 *            a(6k+4) = 34 * k + 20 r = 5: a(6k+5) = 34 * k + 28
 * Range:     u8 n <= 45, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n / 6;
    switch (n % 6) {
    case 0:
        if (k == 0)
            return (seq_t)1;
        else
            return (seq_t)34 * (seq_t)k;
    case 1:
        return (seq_t)34 * (seq_t)k + (seq_t)6;
    case 2:
        return (seq_t)34 * (seq_t)k + (seq_t)12;
    case 3:
        if (k == 0)
            return (seq_t)14;
        else
            return (seq_t)28 * (seq_t)k + (seq_t)16;
    case 4:
        return (seq_t)34 * (seq_t)k + (seq_t)20;
    case 5:
        return (seq_t)34 * (seq_t)k + (seq_t)28;
    default:
        return (seq_t)0; /* unreachable */
    }
}
