/*
 * A385187  Area of the unique primitive Pythagorean triple (a,b,c) such that
 *          (a-b+c)/2 = A002378(n) and its long leg and hypotenuse are
 *          consecutive natural numbers.
 * https://oeis.org/A385187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n^2+n-1)*(2*n^2+2*n-1)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 35, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    seq_t term1 = ns * (ns + 1);                       /* n*(n+1) */
    seq_t term2 = ns * ns + ns - 1;                    /* n^2 + n - 1 */
    seq_t term3 = (ns * ns) * 2 + (ns * 2) - 1;        /* 2*n^2 + 2*n - 1 */
    return term1 * term2 * term3;
}
