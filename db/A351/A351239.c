/*
 * A351239  Numbers M such that 101 * M = 1M1, where 1M1 denotes the
 *          concatenation of 1, M and 1.
 * https://oeis.org/A351239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (10^(6*n-3)+1)/91 Recurrence: a(1) = 11, a(2) = 10989011,
 *            a(n) = 1000001*a(n-1) - 1000000*a(n-2) for n > 2
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;        /* offset behaviour not relevant */

    if (n == 1) return (seq_t)11;
    if (n == 2) return (seq_t)10989011;

    seq_t prev2 = (seq_t)11;           /* a(1)  */
    seq_t prev1 = (seq_t)10989011;     /* a(2)  */

    for (idx_t i = 3; i <= n; ++i) {
        /* a(i) = 1000001*a(i-1) - 1000000*a(i-2) */
        seq_t next = prev1 * (seq_t)1000001 - prev2 * (seq_t)1000000;
        prev2 = prev1;
        prev1 = next;
    }
    return prev1;                      /* a(n) */
}
