/*
 * A306675  Number of permutations p of [2n] having at least one index i with
 *          |p(i)-i| = n.
 * https://oeis.org/A306675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2n)! * sum_{k=1}^{2n} (-1)^{k-1} / k! Recurrence: a(0) =
 *            0 a(n) = (2n)(2n-1)*a(n-1) + (2n-1) for n ≥ 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t prev = (seq_t)0;                /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t mul1 = (seq_t)(2 * i);      /* (2n)  */
        seq_t mul2 = (seq_t)(2 * i - 1);  /* (2n-1)*/
        seq_t temp = prev * mul1;         /* a(n-1)*(2n)   */
        temp = temp * mul2;               /* ...*(2n-1)   */
        temp = temp + mul2;               /* + (2n-1)     */
        prev = temp;                      /* store a(n)    */
    }
    return prev;
}
