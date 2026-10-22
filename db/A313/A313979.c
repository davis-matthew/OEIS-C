/*
 * A313979  Coordination sequence Gal.6.654.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n + 2*C7(n) + C6(n) for n≥1, where C7(n) = # { i in
 *            [2,n] : i%9==3 or i%9==7 }, C6(n) = # { i in [2,n] : i%9==5 }.
 *            a(0) = 1. This yields a(1)=5, a(2)=10, … as in OEIS A313979
 * Range:     u8 n <= 46, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset 0 explicitly */
    if (n == 0) return (seq_t)1;

    /* counts of indices i in [2,n] with required remainders */
    idx_t count3 = 0, count5 = 0, count7 = 0;
    if (n >= 3)  count3 = ((n - 3) / 9) + 1;   /* i ≡ 3 (mod 9) */
    if (n >= 5)  count5 = ((n - 5) / 9) + 1;   /* i ≡ 5 (mod 9) */
    if (n >= 7)  count7 = ((n - 7) / 9) + 1;   /* i ≡ 7 (mod 9) */

    /* base linear part 5*n */
    seq_t result = (seq_t)5 * n;

    /* add the periodic corrections */
    seq_t inv3 = (seq_t)count3;
    seq_t inv5 = (seq_t)count5;
    seq_t inv7 = (seq_t)count7;
    seq_t extra = inv3 * 2 + inv7 * 2 + inv5;
    result += extra;

    return result;
}
