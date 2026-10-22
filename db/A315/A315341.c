/*
 * A315341  Coordination sequence Gal.6.667.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) computed separately for odd and even indices. Odd n (n≥1):
 *            k = (n-1)/2 a(n) = 6 + 36*floor(k/3) + {0,10,22}[k mod 3] Even n
 *            (n≥2): k = n/2 - 3 (for n≥8) a(n) = 32 + 32*floor(k/3) +
 *            {0,12,24}[k mod 3] Special cases: a(0)=1, a(2)=10, a(4)=22,
 *            a(6)=32
 * Range:     u8 n <= 42, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;

    if (n & 1) {              /* odd n */
        idx_t k = (n - 1) / 2;          /* steps after the first odd term */
        idx_t cycles = k / 3;
        idx_t rem = k % 3;
        seq_t result = (seq_t)6 + (seq_t)36 * (seq_t)cycles;
        if (rem == 1)
            result += (seq_t)10;
        else if (rem == 2)
            result += (seq_t)22;
        return result;
    } else {                  /* even n */
        if (n == 2)
            return (seq_t)10;
        if (n == 4)
            return (seq_t)22;
        if (n == 6)
            return (seq_t)32;
        /* n >= 8 */
        idx_t half = n / 2;
        idx_t k = half - 3;           /* steps after the term at n = 6 */
        idx_t cycles = k / 3;
        idx_t rem = k % 3;
        seq_t result = (seq_t)32 + (seq_t)32 * (seq_t)cycles;
        if (rem == 1)
            result += (seq_t)12;
        else if (rem == 2)
            result += (seq_t)24;
        return result;
    }
}
