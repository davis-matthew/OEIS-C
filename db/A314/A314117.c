/*
 * A314117  Coordination sequence Gal.6.330.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n + t(n) where t(n) is the number of indices of the
 *            form s_k = 2 + 11*floor(k/3) + sum_{i=1}^{k%3} d_i (d = {4,4,3})
 *            that are ≤ n. For n < 2, t(n) = 0; for n = 0, a(n) = 1 (special
 *            case)
 * Range:     u8 n <= 48, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                  /* a(0) = 1 */
    if (n < 2)                      /* n == 1 */
        return 5;                  /* a(1) = 5, computed as 5*1 + 0 */

    /* Count of indices s_k ≤ n for n ≥ 2 */
    idx_t q = (n - 2) / 11;                 /* number of complete 11‑step periods */
    idx_t period_base = 2 + 11 * q;         /* s_{3q} */
    idx_t r = 1;                            /* always at least s_{3q} itself */
    if (n >= period_base + 4) r++;          /* s_{3q+1} */
    if (n >= period_base + 8) r++;          /* s_{3q+2} */

    idx_t count = 3 * q + r;                /* total s_k values ≤ n */

    /* a(n) = 5*n + count */
    return (seq_t)n * (seq_t)5 + (seq_t)count;
}
