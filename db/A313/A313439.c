/*
 * A313439  Coordination sequence Gal.4.110.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n + adjustment, depending on n modulo 6. Let k = ⌊n/6⌋.
 *            The adjustment e(n) is: r = n mod 6 r=0 : e = 1 if k ≤ 1 e =
 *            2*k−1 otherwise r=1 or 2 : e = 0 if k = 0 e = 1 otherwise r=3 :
 *            e = –2*(k+1) r=4 or 5 : e = 0 Then a(n) = 5*n + e(n)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n / 6;
    int    r = n % 6;          /* r ∈ [0,5] */
    int64_t e;                /* adjustment, may be negative */

    switch (r) {
    case 0:
        e = (k <= 1) ? 1 : (int64_t)(2 * k - 1);
        break;
    case 1:
    case 2:
        e = (k == 0) ? 0 : 1;
        break;
    case 3:
        e = -2 * ((int64_t)k + 1);
        break;
    default:                   /* r = 4 or 5 */
        e = 0;
        break;
    }

    seq_t res = (seq_t)n * 5;   /* base value 5*n */
    if (e >= 0) {
        res += (seq_t)e;
    } else {
        res -= (seq_t)(-e);
    }
    return res;
}
