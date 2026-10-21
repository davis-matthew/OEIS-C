/*
 * A269486  a(n) = Sum_{j=0..10} (-n)^j.
 * https://oeis.org/A269486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=0..10} (-n)^j
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 84, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence A269486: a(n) = 1 - n + n^2 - n^3 + ... + (-n)^10 */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t term = 1;           /* term = n^j  (unsigned, |j|=0 initially) */
    int sign = 1;             /* +1 for even j, -1 for odd j */

    for (idx_t j = 0; j <= 10; ++j)
    {
        if (sign > 0)
            sum += term;
        else
            sum -= term;

        if (j == 10)          /* last term, exit loop */
            break;

        term *= (seq_t)n;     /* term = n^(j+1) for next iteration */
        sign = -sign;         /* alternate sign */
    }

    return sum;
}
