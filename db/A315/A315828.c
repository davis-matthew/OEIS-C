/*
 * A315828  Coordination sequence Gal.3.6.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Coordination sequence for Gal.3.6.3
 * a(0) = 1
 * For n > 0 let q = n/5 (integer division), r = n % 5
 * a(n) = 18*q            if r == 0
 *        6 + 18*q        if r == 1
 *        6 + 12*q        if r == 2 or r == 3
 *        12 + 18*q       if r == 4
 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    idx_t q = n / 5;
    idx_t r = n % 5;
    seq_t res;

    switch (r) {
        case 0:
            res = (seq_t)18 * (seq_t)q;
            break;
        case 1:
            res = (seq_t)6 + (seq_t)18 * (seq_t)q;
            break;
        case 2:
        case 3:
            res = (seq_t)6 + (seq_t)12 * (seq_t)q;
            break;
        case 4:
            res = (seq_t)12 + (seq_t)18 * (seq_t)q;
            break;
        default:
            res = 0;  /* unreachable, but keeps compiler happy */
    }
    return res;
}
