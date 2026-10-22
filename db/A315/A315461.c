/*
 * A315461  Coordination sequence Gal.5.69.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1. For n > 0 a(n) = 5*n + c(n), where c(n) depends on n
 *            mod 10: c(n) = 0 if n mod 10 = 0 or 5 1 if n mod 10 = 1,2,3,4 -1
 *            if n mod 10 = 6,7,8,9 This implements the observed pattern of
 *            the sequence
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t base = (seq_t)5 * (seq_t)n;
    switch (n % 10) {
        case 1:
        case 2:
        case 3:
        case 4:
            return base + (seq_t)1;
        case 6:
        case 7:
        case 8:
        case 9:
            return base - (seq_t)1;
        default:   /* n % 10 == 0 or 5 */
            return base;
    }
}
