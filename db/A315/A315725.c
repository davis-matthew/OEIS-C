/*
 * A315725  Coordination sequence Gal.6.158.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(1) = 6, a(2) = 12, a(3) = 18 For n ≥ 4: Let s = n - 3
 *            (number of steps from a(3)) Let c = s / 11 (full cycles of 11
 *            deltas) Let r = s % 11 (remaining deltas) Sum of full cycles =
 *            56 (5×4 + 6×6) Sum of remaining deltas = r*4 if r ≤ 5 20 +
 *            (r-5)*6 if r > 5 a(n) = 18 + c*56 + sum_of_remai...
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    if (n <= 3)
        return (seq_t)6 * (seq_t)n; /* n=1→6, n=2→12, n=3→18 */

    seq_t steps = (seq_t)(n - 3);          /* number of deltas after a(3) */
    seq_t cycles = steps / 11;             /* complete 11‑delta cycles */
    seq_t rem    = steps % 11;             /* remaining deltas */

    seq_t sum_rem;
    if (rem <= 5)
        sum_rem = rem * 4;
    else
        sum_rem = 20 + (rem - 5) * 6;      /* 5×4 + (rem‑5)×6 */

    seq_t result = (seq_t)18 + cycles * 56 + sum_rem;
    return result;
}
