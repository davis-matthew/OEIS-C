/*
 * A263883  Ivanov's number a(n) = i*(n+2-i) where i = min{j > 0 | j*(j+1) >=
 *          2*(n-1)}.
 * https://oeis.org/A263883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = i*(n+2-i) where i is the smallest positive integer *
 *            satisfying i*(i+1) >= 2*(n-1). This implementation directly *
 *            computes i by linear search; for the problem's limits (n≈10⁴) *
 *            this is fast enough. The function works for any unsigned * index
 *            n >= 0
 * Range:     u8 n <= 37, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)            /* special case to avoid underflow of 2*(n-1) */
        return (seq_t)1;   /* i = 1, result = 1* (0+2-1) = 1 */

    /* Compute target = 2*(n-1) as seq_t to avoid overflow with large n. */
    seq_t target = (seq_t)2 * ((seq_t)n - 1u);

    /* Find the minimal i > 0 such that i*(i+1) >= target. */
    idx_t i = 1;
    /* Loop guaranteed to finish in O(sqrt(n)) iterations; safe for n ≤ 10⁴. */
    while ((seq_t)i * ((seq_t)i + 1u) < target)
        ++i;

    /* Compute a(n) = i*(n+2-i). */
    return (seq_t)i * (((seq_t)n + 2u) - (seq_t)i);
}
