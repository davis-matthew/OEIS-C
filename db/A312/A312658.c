/*
 * A312658  Coordination sequence Gal.6.249.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n + B(n) Let k = n / 5 (integer division) and r = n %
 *            5. Then B(n) = 4*k + offset(k,r) where offset(0,0) = 1
 *            offset(any,1) = 0 offset(k,2) = (k%2 ? 2 : 0) offset(k,3) = (k%2
 *            ? 4 : 2) offset(any,4) = 4
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n / 5;
    idx_t r = n % 5;
    seq_t base = (seq_t)4 * (seq_t)n;      // 4*n
    seq_t B    = (seq_t)4 * (seq_t)k;      // 4*k
    seq_t off  = 0;

    switch (r) {
    case 0:
        if (k == 0)
            off = 1;
        break;
    case 2:
        if (k & 1)
            off = 2;
        break;
    case 3:
        off = (k & 1) ? 4 : 2;
        break;
    case 4:
        off = 4;
        break;
    default: /* r == 1 */
        break;
    }
    return base + B + off;
}
