/*
 * A334289  Sum of the lengths of all r X s rectangles such that r < s, r + s
 *          = 2n and (s - r) | (s * r).
 * https://oeis.org/A334289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{i=1}^{n-1} (2*n - i) if (i*(2*n - i)) % (2*(n - i)) ==
 *            0 * Complexity: O(n) time, O(1) space
 * Range:     u8 n <= 35, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    seq_t total  = 0;
    seq_t two_n  = (seq_t)n * 2;

    for (idx_t i = 1; i < n; ++i) {
        seq_t r    = (seq_t)i;          /* r = i */
        seq_t s    = two_n - r;         /* s = 2*n - i */
        seq_t diff = s - r;             /* diff = 2*(n-i) */

        /* check (s - r) divides (r * s) */
        if ((r * s) % diff == 0)
            total += s;
    }

    return total;
}
