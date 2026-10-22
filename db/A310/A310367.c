/*
 * A310367  Coordination sequence Gal.6.129.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n + 2*c(n), where c(n) = number of integers k in [1..n]
 *            with k mod 11 == 2 or 10. In effect: a(0)=1 and for n>=1: a(n) =
 *            4*n + 2* ( floor(n/11)*2 + [n%11>=2] + [n%11>=10] )
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)1;         /* a(0) = 1 */

    idx_t full = n / 11u;        /* complete cycles of 11 */
    idx_t rem  = n % 11u;        /* remaining part of the last cycle */

    /* count of k with k≡2 or 10 (mod 11) among 1..n */
    idx_t count6 = full * 2u + (rem >= 2u) + (rem >= 10u);

    return (seq_t)4u * (seq_t)n + (seq_t)2u * (seq_t)count6;
}
