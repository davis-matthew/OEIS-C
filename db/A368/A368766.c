/*
 * A368766  a(n) = n! * (1 + Sum_{k=0..n} (-1)^k * binomial(k+1,2) / k!).
 * https://oeis.org/A368766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * (1 + \sum_{k=0}^n (-1)^k * binomial(k+1,2)/k!)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1; a(n) = n*a(n-1) + (-1)^n * binomial(n+1,2). */

seq_t a(idx_t n)
{
    seq_t prev = (seq_t)1;          /* a(0) = 1 */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* compute n * a(n-1) */
        seq_t prod = ((seq_t)k) * prev;

        /* binomial(k+1,2) = k*(k+1)/2 */
        seq_t bin = ((seq_t)k) * (seq_t)(k + 1) / (seq_t)2;

        if (k & 1)                     /* odd k: (-1)^k = -1 */
            prev = prod - bin;
        else                           /* even k: (-1)^k = +1 */
            prev = prod + bin;
    }

    return prev;
}
