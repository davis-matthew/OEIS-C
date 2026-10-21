/*
 * A265021  Sum of fifth powers of the first n even numbers.
 * https://oeis.org/A265021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (8/3)*n^2*(n+1)^2*(2*n^2+2*n-1)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 30, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t n_t  = n;
    seq_t n2   = n_t * n_t;                 /* n² */
    seq_t np1  = n_t + 1;                   /* n+1 */
    seq_t np12 = np1 * np1;                 /* (n+1)² */
    seq_t base = 8 * n2 * np12;             /* 8 * n² * (n+1)² */
    seq_t inner = 2 * n2 + 2 * n_t - 1;     /* 2*n² + 2*n - 1 */

    /* 3 divides base*inner. Divide one factor to keep numbers small */
    if (base % 3 == 0)
        base /= 3;
    else
        inner /= 3;

    return base * inner;
}
