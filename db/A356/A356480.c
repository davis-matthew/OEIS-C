/*
 * A356480  a(n) is the minimal number of river crossings necessary to solve
 *          the missionaries and cannibals problem for n missionaries and n
 *          cannibals where the boat capacity is the minimum necessary to
 *          allow a solution.
 * https://oeis.org/A356480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1 a(2)=5 a(3)=11 a(n)=2*n+1 for 4 ≤ n ≤ 5 a(n)=2*n-3 for n
 *            ≥ 6
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset: a(1) is first term */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)11;
    if (n <= 5) {                 /* n is 4 or 5 */
        return (seq_t)2 * (seq_t)n + (seq_t)1;
    }
    /* n >= 6 */
    return (seq_t)2 * (seq_t)n - (seq_t)3;
}
