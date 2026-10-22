/*
 * A315256  Coordination sequence Gal.5.111.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>0 let r=n%9, k=n/9, then a(n)=b[r]+44*k, where
 *            b[0]=0 and b[1..8]={6,10,15,20,24,29,34,38}
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* precomputed values for n modulo 9 */
    static const seq_t b[9] = {0, 6, 10, 15, 20, 24, 29, 34, 38};

    if (n == 0)
        return 1;

    idx_t k = n / 9;      /* number of full 9‑step cycles */
    idx_t r = n % 9;      /* remainder within a cycle */
    /* compute a(n) using the linear recurrence a(n)=a(n-9)+44 */
    return b[r] + (seq_t)44 * (seq_t)k;
}
