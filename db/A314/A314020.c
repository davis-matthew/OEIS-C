/*
 * A314020  Coordination sequence Gal.6.247.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(0) = 1 * For n > 0: * k = n / 5, r = n % 5 * if r == 0 :
 *            a(n) = 24 * k * if r == 1 : a(n) = 24 * k + (k % 2 ? 4 : 5) * if
 *            r == 2 : a(n) = 24 * k + (k % 2 ? 9 : 11) * if r == 3 : a(n) =
 *            24 * k + (k % 2 ? 13 : 15) * if r == 4 : a(n) = 24 * k + (k % 2
 *            ? 19 : 20)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    idx_t k = n / 5;
    int r = n % 5;
    seq_t base = ((seq_t)24) * k;

    if (r == 0) {
        return base;
    }

    int extra;
    switch (r) {
        case 1: extra = (k & 1) ? 4 : 5; break;
        case 2: extra = (k & 1) ? 9 : 11; break;
        case 3: extra = (k & 1) ? 13 : 15; break;
        case 4: extra = (k & 1) ? 19 : 20; break;
        default: extra = 0; /* unreachable */ break;
    }

    return base + (seq_t)extra;
}
