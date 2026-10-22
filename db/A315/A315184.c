/*
 * A315184  Coordination sequence Gal.6.450.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(2) = 10; a(2k) = 9k + r, k ≥ 2, where r = { +1 if k
 *            mod 4 = 1, -1 if k mod 4 = 3, 0 otherwise }; a(2k+1) = 9k +
 *            offset, k ≥ 0, where offset = 6 if k mod 4 = 0, 5 + 2·(k/4) if k
 *            mod 4 = 1, 6 + 2·((k-2)/4) if k mod 4 = 2, 5 + 2k if k mod 4 = 3
 * Range:     u8 n <= 46, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n >> 1;              /* floor(n/2) */
    if ((n & 1) == 0) {            /* even n */
        if (k == 0) return (seq_t)1;
        if (k == 1) return (seq_t)10;
        seq_t term = (seq_t)9 * (seq_t)k;
        switch (k & 3) {           /* k mod 4 */
            case 1: term += (seq_t)1; break;
            case 3: term -= (seq_t)1; break;
            default: break;
        }
        return term;
    } else {                       /* odd n */
        switch (k & 3) {           /* k mod 4 */
            case 0:
                return (seq_t)11 * (seq_t)k + (seq_t)6;
            case 1: {
                idx_t q = k / 4;
                return (seq_t)9 * (seq_t)k + ((seq_t)5 + (seq_t)2 * (seq_t)q);
            }
            case 2: {
                idx_t q = (k - 2) / 4;
                return (seq_t)9 * (seq_t)k + ((seq_t)6 + (seq_t)2 * (seq_t)q);
            }
            case 3:
                return (seq_t)11 * (seq_t)k + (seq_t)5;
        }
    }
    /* unreachable */
    return (seq_t)0;
}
