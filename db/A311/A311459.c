/*
 * A311459  Coordination sequence Gal.4.59.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = * let k = n / 6, r = n % 6 * if r==0: a = 1 if k==0 else
 *            26*k * if r==1: a = 26*k + 4 * if r==2: a = 8 if k==0 else 25*k
 *            + 9 * if r==3: a = 26*k + 12 * if r==4: a = 17 if k==0 else 25*k
 *            + 18 * if r==5: a = 26*k + 21 * * This piecewise formula
 *            reproduces the coordination sequence * Gal.4.59...
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n / 6;
    uint8_t r = n % 6;
    seq_t sk = (seq_t)k;

    switch (r) {
        case 0:
            if (k == 0) return (seq_t)1;
            return (seq_t)26 * sk;
        case 1:
            return (seq_t)26 * sk + (seq_t)4;
        case 2:
            if (k == 0) return (seq_t)8;
            return (seq_t)25 * sk + (seq_t)9;
        case 3:
            return (seq_t)26 * sk + (seq_t)12;
        case 4:
            if (k == 0) return (seq_t)17;
            return (seq_t)25 * sk + (seq_t)18;
        case 5:
            return (seq_t)26 * sk + (seq_t)21;
    }
    /* unreachable */
    return (seq_t)0;
}
