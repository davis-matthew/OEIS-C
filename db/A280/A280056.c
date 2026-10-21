/*
 * A280056  Number of 2 X 2 matrices with entries in {0,1,...,n} and even
 *          trace with no entries repeated.
 * https://oeis.org/A280056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((-1)^n + 2*n^2 - 1)*(n-1)*(n-2)/4, using only integer
 *            operations. Works for all n ≥ 0
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 304, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* compute term = (-1)^n + 2*n^2 - 1 (always even) */
    seq_t term;
    if ((n & 1) == 0)          /* n even: (-1)^n = +1 */
        term = nn * nn * 2;     /* 2*n^2  */
    else                       /* n odd:  (-1)^n = -1 */
        term = nn * nn * 2 - 2; /* 2*n^2 - 2 */
    /* compute product and divide by 4 (exact division) */
    seq_t prod = term * (nn - 1);
    prod = prod * (nn - 2);
    return prod / 4;
}
