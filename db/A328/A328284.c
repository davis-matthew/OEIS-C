/*
 * A328284  An extension of the Jacobsthal numbers: 0, 0, 1, followed by
 *          A001045.
 * https://oeis.org/A328284
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=0, a(2)=1, a(3)=0. For n>=4: a(n)=a(n-1)+2*a(n-2).
 *            (Equivalently a(n)=Jacobsthal(n-3) for n>=3.)
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle special initial values */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)0;

    /* for n>=4, compute Jacobsthal(n-3) */
    idx_t k = n - 3;          /* k >= 1 */
    seq_t j_prev2 = (seq_t)0; /* J(0)   */
    seq_t j_prev  = (seq_t)1; /* J(1)   */

    if (k == 0) return j_prev2;
    if (k == 1) return j_prev;

    for (idx_t i = 2; i <= k; ++i) {
        seq_t j_cur = j_prev + (j_prev2 * (seq_t)2);
        j_prev2 = j_prev;
        j_prev = j_cur;
    }
    return j_prev;
}
