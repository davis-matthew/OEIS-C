/*
 * A314107  Coordination sequence Gal.6.205.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n + 2*floor(n/12) + c[n mod 12] – (n mod 12 == 0 ? 1 :
 *            0) where c = {1,0,1,1,1,0,1,2,1,1,1,2}
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

    static const seq_t coeff[12] = {1,0,1,1,1,0,1,2,1,1,1,2};

    idx_t  r = n % 12;          /* remainder modulo 12 */
    idx_t  q = n / 12;          /* quotient on division by 12 */

    /* 5*n as seq_t to avoid overflow in 64‑bit intermediate */
    seq_t five_n = (seq_t)5 * (seq_t)n;

    /* accumulated periodic offset */
    seq_t c = (seq_t)2 * (seq_t)q + coeff[r];

    /* correction for the r==0 case */
    if (r == 0)
        c -= (seq_t)1;

    return five_n + c;
}
