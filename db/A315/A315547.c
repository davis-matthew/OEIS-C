/*
 * A315547  Coordination sequence Gal.5.295.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Coordination sequence Gal.5.295.2 – a(n) counts vertices at
   distance n from a vertex of type 2 in tiling 295.
   The increments a(n)-a(n-1) are 5 at n-1 ≡ 1 or 8 (mod 10) and
   at n-1 = 0; otherwise the increment is 6.  The closed form is
   a(n) = 1 + 5·cnt5 + 6·(n - cnt5) where cnt5 is the number of
   indices i ∈ [0,n-1] with i = 0 or i ≡ 1 or 8 (mod 10).  */
seq_t a(idx_t n)
{
    if (n == 0)
        return 1;             /* a(0) = 1 */

    /* Count indices i (0 ≤ i ≤ n – 1) that produce a 5‑increment. */
    idx_t m = n - 1;           /* largest i used in the sum   */
    idx_t cnt = 1;             /* i = 0 always gives a 5        */

    if (m >= 1)                /* i ≡ 1 (mod 10) part           */
        cnt += ((m - 1) / 10) + 1;
    if (m >= 8)                /* i ≡ 8 (mod 10) part           */
        cnt += ((m - 8) / 10) + 1;

    /* a(n) = 1 + 5·cnt + 6·(n – cnt) */
    return 1 + (seq_t)cnt * 5 + (seq_t)(n - cnt) * 6;
}
