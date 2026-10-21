/*
 * A268219  a(n) = (n!/4!)*Sum(1/k!,k=1..n-4).
 * https://oeis.org/A268219
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n!/4!)*∑(1/k!, k=1..n-4)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for the OEIS sequence A268219.
 * Uses the recurrence:
 *     a(0..4) = 0
 *     a(n) = n*a(n-1) + n*(n-1)*(n-2)*(n-3)/24   for n >= 5
 */

seq_t a(idx_t n)
{
    if (n < 5)
        return 0;

    seq_t prev = 0;
    for (idx_t i = 5; i <= n; ++i)
    {
        seq_t term = (((((seq_t)i * (seq_t)(i - 1)) *
                        (seq_t)(i - 2)) *
                       (seq_t)(i - 3)) /
                      24);
        prev = (seq_t)i * prev + term;
    }
    return prev;
}
