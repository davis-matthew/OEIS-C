/*
 * A319743  Row sums of A174158.
 * https://oeis.org/A319743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{m=1..n} ((C(n-1, m-1) * C(n, m-1) / m)^2)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 20, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Safety for n == 0 – the sequence is only defined for n ≥ 1. */
    if (n == 0) return 0;

    seq_t sum = 0;
    seq_t t = 1;                 /* t for m = 1 : C(n-1,0)*C(n,0)/1 = 1 */

    for (idx_t m = 1; m <= n; ++m)
    {
        /* add the square of the current term to the sum */
        sum += t * t;

        /* compute t for the next m (unless m == n) */
        if (m < n)
        {
            /* recurrence: t_{m+1} = t_m * ((n-m)*(n-m+1)) / (m*(m+1)) */
            seq_t num   = (seq_t)(n - m) * (seq_t)(n - m + 1);
            seq_t denom = (seq_t)m * (seq_t)(m + 1);
            t = (t * num) / denom;
        }
    }

    return sum;
}
