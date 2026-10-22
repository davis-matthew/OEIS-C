/*
 * A308136  Number of (undirected) Hamiltonian paths in the 2n-crossed prism
 *          graph.
 * https://oeis.org/A308136
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*2^n*n*(n+1)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 21, u64 n <= 51, u128 n <= 112,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(2)=72, a(3)=288, a(4)=960
 * a(n) = 6*a(n-1) - 12*a(n-2) + 8*a(n-3) for n>4   */
seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* offset is 2, behaviour below is irrelevant */

    if (n == 2) return 72;
    if (n == 3) return 288;
    if (n == 4) return 960;

    /* Use a sliding window to compute up to n */
    seq_t a_prev3 = 72;   /* a(n-3) */
    seq_t a_prev2 = 288;  /* a(n-2) */
    seq_t a_prev1 = 960;  /* a(n-1) */
    seq_t a_curr;

    for (idx_t i = 5; i <= n; ++i) {
        /* a(n) = 6*a(n-1) - 12*a(n-2) + 8*a(n-3) */
        a_curr = 6*a_prev1 - 12*a_prev2 + 8*a_prev3;
        a_prev3 = a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_prev1;   /* a_prev1 now holds a(n) */
}
