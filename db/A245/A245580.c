/*
 * A245580  Smallest Lucas number L(m) > L(n) that is divisible by the n-th
 *          Lucas number L(n) = A000204(n).
 * https://oeis.org/A245580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = L(3*n) for n >= 2, a(1) = 3 L is the Lucas sequence with
 *            L(0)=2, L(1)=1, L(n)=L(n-1)+L(n-2)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The offset of the sequence is 1; a(1) is the first term. */
    if (n == 1)
        return (seq_t)3;   /* L(2) = 3 */

    /* For n ≥ 2 we need the Lucas number L(3*n). */
    idx_t k = 3 * n;        /* target Lucas index */

    /* Compute the Lucas number iteratively. */
    seq_t L0 = (seq_t)2;    /* L(0) */
    seq_t L1 = (seq_t)1;    /* L(1) */
    seq_t L2;

    /* For k = 0 or 1 we would return directly, but k ≥ 6 here. */
    for (idx_t i = 2; i <= k; ++i)
    {
        L2 = L0 + L1;   /* L(i) = L(i-1) + L(i-2) */
        L0 = L1;
        L1 = L2;
    }

    return L1;   /* L1 holds L(k) after the loop */
}
