/*
 * A360852  Number of induced paths in the n X n rook graph.
 * https://oeis.org/A360852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -n^2 + (n!^2) * ∑_{k=0}^{n-1} ((1 + k) / (k!^2))
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is well-defined for all n ≥ 1 and is computed by the recurrence */
/* T_{k-1} = k^3/(k+1) * T_k, with T_{n-1} = n^3.                        */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset: a(1) is the first term */
    seq_t term = (seq_t)n * (seq_t)n * (seq_t)n;   /* term = n^3 = T_{n-1}       */
    seq_t sum  = term;                         /* sum of all terms so far   */
    for (idx_t k = n; k > 1; --k) {
        idx_t kk = k - 1;                      /* actual index k in formula */
        seq_t mul = (seq_t)kk * (seq_t)kk;     /* kk^2 */
        mul *= (seq_t)kk;                      /* kk^3 */
        term = term * mul;                     /* multiply by k^3            */
        term = term / (kk + 1);                /* divide by (k+1), exact      */
        sum += term;                           /* accumulate term T_{k-1}    */
    }
    seq_t nn = (seq_t)n;
    return sum - nn * nn;                       /* a(n) = total - n^2         */
}
