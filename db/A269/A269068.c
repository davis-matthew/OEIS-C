/*
 * A269068  a(n+2) = a(n+1) + L(n+1)*a(n), where L = Lucas number (A000032)
 *          and a(0) = a(1) = 1.
 * https://oeis.org/A269068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+2) = a(n+1) + L(n+1)*a(n), a(0) = a(1) = 1 where L(k) are
 *            Lucas numbers: L(0)=2, L(1)=1, L(k)=L(k-1)+L(k-2)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 18, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    /* iterative computation */
    seq_t a0 = (seq_t)1;          /* a(n-2)   */
    seq_t a1 = (seq_t)1;          /* a(n-1)   */
    seq_t lucPrevPrev = (seq_t)2; /* L(n-2), starting with L0 */
    seq_t lucPrev     = (seq_t)1; /* L(n-1), starting with L1 */

    for (idx_t i = 2; i <= n; ++i) {
        /* a(i) = a(i-1) + L(i-1)*a(i-2) */
        seq_t next = a1 + lucPrev * a0;

        /* prepare next Lucas number: L(i) = L(i-1)+L(i-2) */
        seq_t lucNext = lucPrev + lucPrevPrev;
        lucPrevPrev = lucPrev;
        lucPrev = lucNext;

        /* shift a's for next iteration */
        a0 = a1;
        a1 = next;
    }
    return a1;
}
