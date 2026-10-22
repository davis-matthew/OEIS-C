/*
 * A315905  Coordination sequence Gal.5.271.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) for n ≥ 0 is given piecewise by the residue of n modulo 6.
 *            Let t = n / 6. a(6t) = 28t (t > 0), a(0) = 1 a(6t+1) = 6 + 36t
 *            a(6t+2) = 8 (t = 0), a(6t+2) = 22t + 12 (t > 0) a(6t+3) = 14 +
 *            36t a(6t+4) = 18 + 22t a(6t+5) = 26 + 36t
 * Range:     u8 n <= 42, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t t = n / 6;
    switch (n % 6) {
        case 0:
            if (t == 0) return (seq_t)1;
            return (seq_t)28 * (seq_t)t;
        case 1:
            return (seq_t)6 + (seq_t)36 * (seq_t)t;
        case 2:
            if (t == 0) return (seq_t)8;
            return (seq_t)22 * (seq_t)t + (seq_t)12;
        case 3:
            return (seq_t)14 + (seq_t)36 * (seq_t)t;
        case 4:
            return (seq_t)18 + (seq_t)22 * (seq_t)t;
        case 5:
            return (seq_t)26 + (seq_t)36 * (seq_t)t;
        default:   /* unreachable */
            return (seq_t)0;
    }
}
