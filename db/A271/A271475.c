/*
 * A271475  Total number of burnt pancakes flipped using the Max(n) greedy
 *          algorithm.
 * https://oeis.org/A271475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = (n * n!)/2 + Sum_{k=2..n-1} (k^2 * k!)/2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)                 /* a(1) = 0 */
        return 0;

    seq_t sum  = 0;
    seq_t fact = 1;
    idx_t i;

    for (i = 1; i < n; ++i)
    {
        fact *= (seq_t)i;        /* fact = i! */

        if (i >= 2)              /* add term for k = i */
        {
            seq_t term = ((seq_t)i * (seq_t)i) * fact / 2;
            sum += term;
        }
    }

    /* fact now equals (n-1)!; multiply by n to get n! */
    fact *= (seq_t)n;
    sum += (seq_t)n * fact / 2;   /* add the k = n term */

    return sum;
}
