/*
 * A293146  a(n) = n! * [x^n] exp(x/(1 - n*x)).
 * https://oeis.org/A293146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * [x^n] exp(x/(1 - n*x)) Equivalent recurrence: a(0) =
 *            1 a(n) = 1 + Σ_{r=0}^{n-1} t_r where t_0 = 1 and t_{r+1} = t_r *
 *            n * (n - r - 1) * (n - r) / (r + 1) (for n ≥ 1) This directly
 *            evaluates a(n) in O(n) time
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t ans   = (seq_t)1; /* terms from r = 0 ... */
    seq_t term  = (seq_t)1; /* t_0  */

    for (idx_t r = 0; r < n; ++r)
    {
        seq_t num = term * (seq_t)n;                /* multiply by n   */
        num = num * (seq_t)(n - r - 1);             /* multiply by (n-r-1) */
        num = num * (seq_t)(n - r);                 /* multiply by (n-r)   */
        seq_t den = (seq_t)(r + 1);                 /* divide by (r+1)     */

        term = num / den;                           /* exact division   */
        ans += term;                                /* accumulate sum   */
    }

    return ans;
}
