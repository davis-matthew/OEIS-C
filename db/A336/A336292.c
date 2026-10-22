/*
 * A336292  a(n) = (n!)^2 * Sum_{k=1..n} (-1)^(n-k) / (k * ((n-k)!)^2).
 * https://oeis.org/A336292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (n!)^2 * Sum_{k=1..n} (-1)^(n-k) / (k * ((n-k)!)^2)
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 8, i64 n <= 13, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) = 0 */
    if (n == 0) return 0;

    seq_t sum = 0;             /* running sum */
    seq_t prev_fact = 1;       /* (k-1)!  for current k */
    seq_t comb = 1;            /* C(n,0)  initially */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* update binomial coefficient C(n,k) */
        comb = comb * (seq_t)(n - k + 1) / (seq_t)k;

        /* current factorial k! */
        seq_t cur_fact = prev_fact * (seq_t)k;

        /* term = k * ((k-1)!)^2 * C(n,k)^2  (always an integer) */
        seq_t term = prev_fact * prev_fact;        /* (k-1)!^2   */
        term = term * (comb * comb);              /* * C(n,k)^2  */
        term = term * (seq_t)k;                   /* * k          */

        /* add with sign (-1)^(n-k) */
        if ( ((n - k) & 1) == 0 )
            sum += term;
        else
            sum -= term;

        prev_fact = cur_fact;                     /* advance to next (k!) */
    }
    return sum;
}
