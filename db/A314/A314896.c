/*
 * A314896  Coordination sequence Gal.4.53.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 5*n - (#k ≤ n with k==1 or k%8==2 or k%8==7)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The number of “4”‑steps up to n is 1 (for k=1) plus the counts of indices
   congruent to 2 or 7 (mod 8).  Then a(n)=1+5n−cnt4. */

seq_t a(idx_t n)
{
    seq_t five_n = (seq_t)n * 5;       /* 5*n  */
    seq_t cnt4   = 0;                  /* number of k≤n with inc(k)=4 */

    if (n >= 1)            cnt4 += 1;                  /* k = 1 */
    if (n >= 2)            cnt4 += (seq_t)(((n - 2) / 8) + 1);
    if (n >= 7)            cnt4 += (seq_t)(((n - 7) / 8) + 1);

    return 1 + five_n - cnt4;          /* a(0)=1, a(n)=1+5n‑cnt4 for n≥0 */
}
