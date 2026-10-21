/*
 * A240926  Duplicate of A092387.
 * https://oeis.org/A240926
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 3*a(n-1) - a(n-2) - 2, with a(0) = 4, a(1) = 5
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 22, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 4;
    if (n == 1) return 5;

    seq_t prev2 = 4;  /* a(0) */
    seq_t prev1 = 5;  /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur = 3 * prev1 - prev2 - 2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
