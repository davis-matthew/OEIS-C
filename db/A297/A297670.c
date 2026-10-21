/*
 * A297670  Number of chordless cycles in the n-triangular graph.
 * https://oeis.org/A297670
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=4..n} n!/(2*k*(n-k)!)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 4 the sequence is defined to be 0. */
    if (n < 4)
        return 0;

    /* Start with the term for k = 4:
       T_4 = n! / (2*4*(n-4)!) = (n*(n-1)*(n-2)*(n-3)) / 8 */
    seq_t cur = (seq_t)n * (n-1) * (n-2) * (n-3);
    cur /= (seq_t)8;

    seq_t total = cur;

    /* Recurrence:
         T_{k+1} = T_k * k * (n-k) / (k+1)
     Using the order * (n-k) -> / (k+1) -> * k prevents
     overflow for all terms that fit in seq_t. */
    for (idx_t k = 4; k <= n-1; ++k)
    {
        seq_t temp = cur * (seq_t)(n - k);     /* (n-k) is always <= n */
        temp /= (seq_t)(k + 1);
        temp *= (seq_t)k;
        cur   = temp;
        total += cur;
    }

    return total;
}
