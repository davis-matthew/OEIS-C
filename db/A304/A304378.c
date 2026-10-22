/*
 * A304378  a(n) = 4*(n - 1)*(16*n - 23) for n >= 1.
 * https://oeis.org/A304378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*(n - 1)*(16*n - 23)
 * Range:     u8 n <= 3, u16 n <= 33, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* behaviour below offset is undefined, but return 0 */

    seq_t N = (seq_t)n;
    seq_t temp = 16 * N - 23;      /* 16*n - 23  */
    return 4 * (N - 1) * temp;     /* 4*(n-1)*(16*n-23) */
}
