/*
 * A313568  Coordination sequence Gal.3.14.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 24 * floor(n/5) + [1,5,10,14,19][n mod 5] - (n mod 5 == 0
 *            && n>0 ? 1 : 0)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t coeff[5] = {
        (seq_t)1,  /* n mod 5 == 0 */
        (seq_t)5,  /* n mod 5 == 1 */
        (seq_t)10, /* n mod 5 == 2 */
        (seq_t)14, /* n mod 5 == 3 */
        (seq_t)19  /* n mod 5 == 4 */
    };
    idx_t k = n / 5;
    idx_t r = n % 5;
    seq_t res = (seq_t)24 * (seq_t)k + coeff[r];
    if (r == 0 && k > 0)
        res -= (seq_t)1;
    return res;
}
