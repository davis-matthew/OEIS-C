/*
 * A280059  Number of 2 X 2 matrices having all elements in {-n,..,0,..,n}
 *          with determinant = permanent.
 * https://oeis.org/A280059
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*(n+1)^3 - 28*(n+1)^2 + 16*(n+1) - 3 for n>0 a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 644, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    seq_t t  = (seq_t)n + 1;     /* (n+1) */
    seq_t t2 = t * t;            /* (n+1)^2 */
    seq_t t3 = t2 * t;           /* (n+1)^3 */
    return 16 * t3 - 28 * t2 + 16 * t - 3;
}
