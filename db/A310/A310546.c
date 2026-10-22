/*
 * A310546  Coordination sequence Gal.4.128.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(1) = 4 For odd n > 1, let k = (n-1)/2: a(n) = 10*k +
 *            (k%2==0 ? 6 : 8) For even n > 0, let k = n/2: if k%2==0 (i.e., k
 *            even and k≥2): a(n) = 10*k - 2 else (k odd): a(n) = 10*k +
 *            4*((k-1)/2)
 * Range:     u8 n <= 45, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;

    if (n & 1u) {                   /* odd n */
        if (n == 1u) return (seq_t)4;
        idx_t k = (n - 1u) >> 1;   /* k = (n-1)/2 */
        seq_t res = (seq_t)10 * (seq_t)k;
        if ((k & 1u) == 0u)
            res += (seq_t)6;
        else
            res += (seq_t)8;
        return res;
    } else {                        /* even n */
        idx_t k = n >> 1;           /* k = n/2 */
        seq_t res = (seq_t)10 * (seq_t)k;
        if ((k & 1u) == 0u) {       /* k even (k≥2) */
            res -= (seq_t)2;
        } else {                    /* k odd */
            seq_t diff = (seq_t)4 * (seq_t)((k - 1u) >> 1);
            res += diff;
        }
        return res;
    }
}
