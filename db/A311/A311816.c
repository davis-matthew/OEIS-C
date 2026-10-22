/*
 * A311816  Coordination sequence Gal.5.95.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 3*(n>=1) + 5*# {k∈[1,n] | k%9==3 or k%9==7} + 4*(n -
 *            (n>=1) - # {k∈[1,n] | k%9==3 or k%9==7})
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;                 /* a(0) = 1 */

    /* count of the single “3” difference at k = 1 */
    idx_t c3 = 1;                         /* 1 if n>=1, otherwise 0 – n!=0 here */

    /* counts of the “5” differences, occurring at k≡3 (mod 9) and k≡7 (mod 9) */
    idx_t c5 = 0;
    if (n >= 3) c5 += (n - 3) / 9 + 1;     /* 3,12,21,… */
    if (n >= 7) c5 += (n - 7) / 9 + 1;     /* 7,16,25,… */

    /* remaining differences are all “4” */
    idx_t c4 = n - c3 - c5;

    return 1 +
           (seq_t)3 * c3 +
           (seq_t)5 * c5 +
           (seq_t)4 * c4;
}
