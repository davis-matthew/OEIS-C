/*
 * A313794  Coordination sequence Gal.6.619.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n + f(n) where f(n) = count of integers m ∈ [1..⌊n/4⌋]
 *            with m not divisible by 3 + count of integers m ∈ [1..⌊(n-1)/4⌋]
 *            with m not divisible by 3
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;           /* a(0) = 1 */

    /* Basis part */
    seq_t base = (seq_t)5 * n;      /* 5*n */

    /* Extra part */
    idx_t m1 = n / 4;              /* ⌊n/4⌋   */
    idx_t m2 = (n - 1) / 4;        /* ⌊(n-1)/4⌋ */

    seq_t e1 = (seq_t)m1 - (seq_t)(m1 / 3);  /* numbers in 1..m1 not divisible by 3 */
    seq_t e2 = (seq_t)m2 - (seq_t)(m2 / 3);  /* numbers in 1..m2 not divisible by 3 */

    return base + e1 + e2;
}
