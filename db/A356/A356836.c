/*
 * A356836  Coordination sequence of the {5,3,4} hyperbolic honeycomb.
 * https://oeis.org/A356836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*A095004(n) + 2 where A095004 satisfies: A095004(0) = 0
 *            A095004(1) = 1 A095004(n) = 8*A095004(n-1) - A095004(n-2) + 2 (n
 *            ≥ 2) and a(0) = 1 (special offset)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) */
    if (n == 1)
        return (seq_t)12;         /* 10*A095004(1)+2 */

    seq_t prev2 = (seq_t)0;       /* A095004(0) */
    seq_t prev1 = (seq_t)1;       /* A095004(1) */
    seq_t cur = 0;

    for (idx_t i = 2; i <= n; ++i) {
        cur = (seq_t)8 * prev1 - prev2 + (seq_t)2; /* recurrence */
        prev2 = prev1;
        prev1 = cur;
    }

    return (seq_t)10 * prev1 + (seq_t)2;
}
