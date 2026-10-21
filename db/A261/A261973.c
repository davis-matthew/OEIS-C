/*
 * A261973  The first of three consecutive positive integers the sum of the
 *          squares of which is equal to the sum of the squares of eleven
 *          consecutive positive integers.
 * https://oeis.org/A261973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 47*a(n-1) - 47*a(n-2) + a(n-3) (n > 3) Initial values:
 *            a(1) = 137, a(2) = 6341, a(3) = 291593 a(0) is undefined; this
 *            implementation returns 0 for n == 0
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined, return 0 to avoid crash */

    /* Handle the first three terms directly. */
    if (n == 1)
        return (seq_t)137;
    if (n == 2)
        return (seq_t)6341;
    if (n == 3)
        return (seq_t)291593;

    /* Iteratively compute a(n) using the recurrence.
       a1 = a(n‑3), a2 = a(n‑2), a3 = a(n‑1). */
    seq_t a1 = (seq_t)137;      /* a(n-3) for current n=4 */
    seq_t a2 = (seq_t)6341;     /* a(n-2) for current n=4 */
    seq_t a3 = (seq_t)291593;   /* a(n-1) for current n=4 */

    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t next = (seq_t)47 * a3 - (seq_t)47 * a2 + a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
