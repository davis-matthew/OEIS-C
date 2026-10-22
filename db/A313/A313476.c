/*
 * A313476  Coordination sequence Gal.5.53.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 for 1 ≤ n ≤ 8: a(n) = 1 + 4*n + 1 for n ≥ 9: a(n) = 1 +
 *            4*n + 1 + 4*floor((n-9)/10) + min(4, ((n-9)%10) + 1) (The
 *            additional terms account for the indices where the increment is
 *            5 instead of 4.) Alternatively: a(n) = 4*n + 2 +
 *            4*floor((n-9)/10) + min(4, ((n-9)%10) + 1) for n≥9
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Baseline: 1 + 4*n */
    seq_t val = (seq_t)1 + (seq_t)n * 4;

    if (n <= 1)
        return val;          /* n = 1 → 5 */

    if (n <= 8)
        return val + (seq_t)1; /* extra 1 for indices 2…8 */

    /* n ≥ 9 */
    idx_t m = (n - 9) / 10;                     /* number of complete 10‑step blocks after 9 */
    idx_t r = (n - 9) % 10;                     /* position inside the current block */

    /* Extra contributions:
       – 1 for the increment at index 2
       – 4 per complete block
       – r+1 extra steps within the current block if r ≤ 3,
         otherwise 4 extra steps. */
    seq_t extra = 1 + (seq_t)(4 * m);
    seq_t partial = (r <= 3) ? (seq_t)(r + 1) : (seq_t)4;

    return val + extra + partial;
}
