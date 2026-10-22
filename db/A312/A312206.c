/*
 * A312206  Coordination sequence Gal.6.157.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n>0, a(n) = 5*n + f(n) where f(n) is a 12‑periodic
 *            sequence ( indexed n≡1,2,…,12 (mod 12) ) f =
 *            [-1,-2,-2,-1,0,0,0,1,2,2,1,0]
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)              /* special case */
        return 1;

    /* 12‑periodic adjustment, given for n≡1…12 (mod 12) */
    static const int adj[12] = {
        -1, -2, -2, -1, 0, 0, 0, 1, 2, 2, 1, 0
    };

    /* index into the pattern: (n‑1) modulo 12 */
    idx_t mod = (n - 1) % 12;

    /* base value 5*n (performed in seq_t) */
    seq_t base = ((seq_t)n) * 5;

    /* apply the adjustment, taking care of sign */
    int a_adj = adj[mod];
    if (a_adj >= 0)
        return base + (seq_t)a_adj;
    else
        return base - (seq_t)(-a_adj);
}
