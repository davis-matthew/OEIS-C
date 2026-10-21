/*
 * A274756  Values of n such that 2*n+1 and 6*n+1 are both triangular numbers.
 * https://oeis.org/A274756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A274756:Values of n such that 2*n+1 and 6*n+1 are both triangular
   Recurrence: a(n) = a(n-1)+37634*a(n-2)-37634*a(n-3)-a(n-4)+a(n-5)
   Initial values: a(1)=0, a(2)=945, a(3)=13167, a(4)=35578242, a(5)=495540990
*/
seq_t a(idx_t n)
{
    /* Handle trivial or small indices directly */
    if (n == 0) return (seq_t)0;           /* undefined for n<1, return 0 */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)945;
    if (n == 3) return (seq_t)13167;
    if (n == 4) return (seq_t)35578242;
    if (n == 5) return (seq_t)495540990;

    /* Start from the known five terms and build up */
    seq_t prev5 = (seq_t)0;                 /* a(1) */
    seq_t prev4 = (seq_t)945;               /* a(2) */
    seq_t prev3 = (seq_t)13167;             /* a(3) */
    seq_t prev2 = (seq_t)35578242;          /* a(4) */
    seq_t prev1 = (seq_t)495540990;         /* a(5) */
    seq_t cur;

    for (idx_t i = 6; i <= n; ++i) {
        /* recurrence: a(i) = a(i-1)+37634*a(i-2)-37634*a(i-3)-a(i-4)+a(i-5) */
        cur = prev1
            + ((seq_t)37634) * prev2
            - ((seq_t)37634) * prev3
            - prev4
            + prev5;

        /* shift the window */
        prev5 = prev4;
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return cur;
}
