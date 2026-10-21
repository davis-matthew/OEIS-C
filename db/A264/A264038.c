/*
 * A264038  Convolution of Lucas and Jacobsthal numbers.
 * https://oeis.org/A264038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1)+2*a(n-2)-3*a(n-3)-2*a(n-4) for n>3, with a(0)=0,
 *            a(1)=2, a(2)=3, a(3)=10
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n == 0ULL)
        return (seq_t)0;
    if (n == 1ULL)
        return (seq_t)2;
    if (n == 2ULL)
        return (seq_t)3;
    if (n == 3ULL)
        return (seq_t)10;

    /* Use four rolling variables to compute the recurrence in O(n). */
    seq_t a0 = (seq_t)0;  /* a(n-4)   */
    seq_t a1 = (seq_t)2;  /* a(n-3)   */
    seq_t a2 = (seq_t)3;  /* a(n-2)   */
    seq_t a3 = (seq_t)10; /* a(n-1)   */
    seq_t next;

    /* Compute from i = 4 up to n inclusive. */
    for (idx_t i = 4ULL; i <= n; ++i) {
        next = (seq_t)2 * a3
             + (seq_t)2 * a2
             - (seq_t)3 * a1
             - (seq_t)2 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;
}
