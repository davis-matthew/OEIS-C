/*
 * A315943  Coordination sequence Gal.5.229.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(n)=6n for n≡0 (mod 3) and n>0; a(n)=6+14*(n-1)/3 for
 *            n≡1 (mod 3); a(n)=8+14*(n-2)/3 for n≡2 (mod 3)
 * Range:     u8 n <= 44, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    idx_t r = n % 3;
    seq_t m = (seq_t)n;

    if (r == 0)                /* n ≡ 0 (mod 3) */
        return m * (seq_t)6;
    else if (r == 1) {         /* n ≡ 1 (mod 3) */
        idx_t k = (n - 1) / 3;
        return (seq_t)6 + (seq_t)14 * (seq_t)k;
    } else {                   /* r == 2, n ≡ 2 (mod 3) */
        idx_t k = (n - 2) / 3;
        return (seq_t)8 + (seq_t)14 * (seq_t)k;
    }
}
