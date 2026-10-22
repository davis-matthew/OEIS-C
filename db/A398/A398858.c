/*
 * A398858  Lower (4/3, 1)-midsequence of triangular numbers (A000217) and
 *          pentagonal numbers (A000326); see Comments.
 * https://oeis.org/A398858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - 2*a(n-4) + a(n-5), a(0) = 0,
 *            a(1) = 2, a(2) = 9, a(3) = 20, a(4) = 35
 * Range:     u8 n <= 10, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases for the first five terms */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)9;
    if (n == 3) return (seq_t)20;
    if (n == 4) return (seq_t)35;

    /* Iteratively compute terms from 5 up to n */
    seq_t v0 = (seq_t)0;   /* a(n-5) */
    seq_t v1 = (seq_t)2;   /* a(n-4) */
    seq_t v2 = (seq_t)9;   /* a(n-3) */
    seq_t v3 = (seq_t)20;  /* a(n-2) */
    seq_t v4 = (seq_t)35;  /* a(n-1) */
    seq_t v5;              /* a(n) */

    for (idx_t i = 5; i <= n; ++i)
    {
        v5 = 2 * v4 - v3 + v2 - 2 * v1 + v0;  /* recurrence */
        v0 = v1;
        v1 = v2;
        v2 = v3;
        v3 = v4;
        v4 = v5;
    }
    return v4;  /* a(n) */
}
