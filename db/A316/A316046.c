/*
 * A316046  Coordination sequence Gal.3.58.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A316046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = if n < 6: {1, 6, 9, 15, 21, 27} else: let k = n / 6, r =
 *            n % 6 r = 0: 30k r = 1: 36k + 6 r = 2: 30k + 12 r = 3: 36k + 15
 *            r = 4: 30k + 24 r = 5: 36k + 27 (this holds for all n >= 6)
 * Range:     u8 n <= 42, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first six terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)9;
    if (n == 3) return (seq_t)15;
    if (n == 4) return (seq_t)21;
    if (n == 5) return (seq_t)27;

    /* for n >= 6: compute by the piecewise linear formula */
    idx_t k = n / 6;
    idx_t r = n % 6;
    seq_t res;

    switch (r) {
        case 0: res = (seq_t)30 * (seq_t)k;           break;
        case 1: res = (seq_t)36 * (seq_t)k + (seq_t)6;break;
        case 2: res = (seq_t)30 * (seq_t)k + (seq_t)12;break;
        case 3: res = (seq_t)36 * (seq_t)k + (seq_t)15;break;
        case 4: res = (seq_t)30 * (seq_t)k + (seq_t)24;break;
        case 5: res = (seq_t)36 * (seq_t)k + (seq_t)27;break;
        default: res = (seq_t)0; /* unreachable */
    }

    return res;
}
