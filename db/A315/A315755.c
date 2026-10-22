/*
 * A315755  Coordination sequence Gal.6.333.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=6; For n≥2, if n mod 10 is 5 or 6 then
 *            a(n)=a(n-1)+4; otherwise a(n)=a(n-1)+6
 * Range:     u8 n <= 45, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms explicitly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;

    /* Count how many indices k (2 ≤ k ≤ n) have k ≡ 5 or 6 (mod 10).  */
    idx_t cnt = 0;
    if (n >= 5) cnt += ((n - 5) / 10) + 1;  /* numbers ≡ 5 (mod 10) */
    if (n >= 6) cnt += ((n - 6) / 10) + 1;  /* numbers ≡ 6 (mod 10) */

    /* Each such index reduces the expected +6 to +4, i.e. subtracts 2 overall. */
    return (seq_t)6 * n - (seq_t)2 * cnt;
}
