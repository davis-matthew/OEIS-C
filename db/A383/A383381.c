/*
 * A383381  Expansion of e.g.f. exp(-2*x) / (1-x)^5.
 * https://oeis.org/A383381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 3; a(n) = (n+2)*a(n-1) + 2*(n-1)*a(n-2) for n ≥
 *            2 (Uses the recurrence given in OEIS A383381.) This
 *            implementation computes the value on each call using an O(n)
 *            iterative loop, so it works for any order of calls without
 *            caching
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    /* a0 = a(n‑2), a1 = a(n‑1) */
    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)3;

    /* Compute successive terms up to n. */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t term1 = (seq_t)(i + 2) * a1;
        seq_t term2 = (seq_t)2 * (seq_t)(i - 1) * a0;
        seq_t ai    = term1 + term2;
        a0 = a1;
        a1 = ai;
    }
    return a1;
}
