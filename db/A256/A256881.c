/*
 * A256881  a(n) = n!/ceiling(n/2).
 * https://oeis.org/A256881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! / ceil(n/2)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Ceil(n/2) for positive integer n */
    seq_t d = ((seq_t)n + 1) / 2;   /* (n+1)/2 is integer division */

    seq_t result = 1;
    for (idx_t i = 1; i <= n; ++i)
    {
        if ((seq_t)i == d)            /* skip the factor that would be divided out */
            continue;
        result *= (seq_t)i;           /* multiply all remaining factors */
    }
    return result;
}
