/*
 * A311345  Coordination sequence Gal.4.59.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n + g(n) with n = 6*k + r (0 <= r < 6) g(6k) = 1 if k
 *            == 0, else 2*k g(6k+1) = 2*k g(6k+2) = 2*k g(6k+3) = 0 if k ==
 *            0, else 2 g(6k+4) = 2*k g(6k+5) = 2*k + 2 This piecewise formula
 *            reproduces all terms of OEIS sequence A311345
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base part: 4*n */
    seq_t base = (seq_t)n * 4;

    /* decompose n = 6*k + r */
    idx_t k = n / 6;
    int r = (int)(n % 6);

    seq_t extra;
    switch (r) {
        case 0:
            extra = (k == 0) ? 1 : 2 * (seq_t)k;
            break;
        case 1:
        case 2:
        case 4:
            extra = 2 * (seq_t)k;
            break;
        case 3:
            extra = (k == 0) ? 0 : 2;
            break;
        case 5:
            extra = 2 * (seq_t)k + 2;
            break;
        default:
            extra = 0;  /* unreachable */
            break;
    }

    return base + extra;
}
