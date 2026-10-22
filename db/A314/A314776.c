/*
 * A314776  Coordination sequence Gal.6.478.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(1) = 5 a(2) = 9 a(3) = 13 For n ≥ 4, with q = n/4,
 *            a(4q) = 20*q a(4q+1) = 18*q + 5 a(4q+2) = 18*q + 9 a(4q+3) =
 *            18*q + 13
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
    if (n == 2) return (seq_t)9;
    if (n == 3) return (seq_t)13;

    idx_t q = n / 4;          /* integer division */
    seq_t val;
    switch (n % 4) {
    case 0:
        val = (seq_t)20 * (seq_t)q;
        break;
    case 1:
        val = (seq_t)18 * (seq_t)q + (seq_t)5;
        break;
    case 2:
        val = (seq_t)18 * (seq_t)q + (seq_t)9;
        break;
    case 3:
        val = (seq_t)18 * (seq_t)q + (seq_t)13;
        break;
    default:
        val = (seq_t)0; /* unreachable */
        break;
    }
    return val;
}
