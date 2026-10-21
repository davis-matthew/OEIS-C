/*
 * A250791  Number of (n+1) X (2+1) 0..1 arrays with nondecreasing
 *          min(x(i,j),x(i,j-1)) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 3*a(n-2) - 2*a(n-3) + 2*a(n-4), with a(1)=24,
 *            a(2)=66, a(3)=180, a(4)=490. This direct recurrence computes any
 *            term in linear time
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 41, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* undefined for n<1; return 0 as guard */

    /* Beginning values */
    seq_t a1 = 24; /* a(1) */
    seq_t a2 = 66; /* a(2) */
    seq_t a3 = 180;/* a(3) */
    seq_t a4 = 490;/* a(4) */

    if (n == 1) return a1;
    if (n == 2) return a2;
    if (n == 3) return a3;
    if (n == 4) return a4;

    /* Recurrence: keep sliding window of last four values */
    seq_t prev4 = a1; /* a(n-4) */
    seq_t prev3 = a2; /* a(n-3) */
    seq_t prev2 = a3; /* a(n-2) */
    seq_t prev1 = a4; /* a(n-1) */
    seq_t next;

    for (idx_t i = 5; i <= n; ++i) {
        /* next = 4*prev1 - 3*prev2 - 2*prev3 + 2*prev4  */
        next = 4 * prev1 - 3 * prev2 - 2 * prev3 + 2 * prev4;
        /* shift for next iteration */
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = next;
    }
    return next;
}
