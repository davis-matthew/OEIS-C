/*
 * A315521  Coordination sequence Gal.6.620.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1. For n > 0, a(n) = 60 * floor(n/11) + c[n % 11], where
 *            c[0] = 0, c[1] = 6, c[2] = 11, c[3] = 17, c[4] = 22, c[5] = 28,
 *            c[6] = 32, c[7] = 38, c[8] = 43, c[9] = 49, c[10] = 54. This
 *            encodes the recurrence a(n) = a(n-11)+60 for n ≥ 11
 * Range:     u8 n <= 46, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t r = n % 11;
    idx_t q = (n - r) / 11;

    seq_t base = (seq_t)0;
    switch (r) {
        case 0:  base = (seq_t)0;   break;   /* base part is 0 for r==0 (handled by n==0 case) */
        case 1:  base = (seq_t)6;   break;
        case 2:  base = (seq_t)11;  break;
        case 3:  base = (seq_t)17;  break;
        case 4:  base = (seq_t)22;  break;
        case 5:  base = (seq_t)28;  break;
        case 6:  base = (seq_t)32;  break;
        case 7:  base = (seq_t)38;  break;
        case 8:  base = (seq_t)43;  break;
        case 9:  base = (seq_t)49;  break;
        case 10: base = (seq_t)54;  break;
    }

    return base + (seq_t)60 * (seq_t)q;
}
