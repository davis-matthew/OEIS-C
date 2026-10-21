/*
 * A288219  a(n) = a(n-1) + a(n-2) for n >= 3, where a(0) = 2, a(1) = 4, a(2)
 *          = 7.
 * https://oeis.org/A288219
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) (n >= 3), a(0)=2, a(1)=4, a(2)=7
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A288219 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)2;
    if (n == 1)
        return (seq_t)4;
    if (n == 2)
        return (seq_t)7;

    /* Initialize with a(1) and a(2) for the recurrence */
    seq_t prev2 = (seq_t)4; /* a(1) */
    seq_t prev1 = (seq_t)7; /* a(2) */

    for (idx_t i = (idx_t)3; i <= n; ++i) {
        seq_t cur = prev1 + prev2; /* a(i) = a(i-1) + a(i-2) */
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1; /* a(n) */
}
