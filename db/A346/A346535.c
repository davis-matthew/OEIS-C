/*
 * A346535  Numbers obtained by adding the first k repdigits that consist of
 *          the same digit, for some number k.
 * https://oeis.org/A346535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = d * Σ_{i=1}^{m} (10^i – 1)/9 with d = (n-1)%9 + 1 and m =
 *            ceil(n/9)
 * Range:     u8 n <= 20, u16 n <= 41, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)        // offset is 1; n=0 is not used
        return (seq_t)0;

    /* compute m = ceil(n/9) */
    idx_t m = (n + 8) / 9;               /* integer division */
    unsigned int d = (unsigned int)((n - 1) % 9 + 1);

    seq_t repunit = (seq_t)0;
    seq_t sum     = (seq_t)0;

    for (idx_t i = 1; i <= m; ++i) {
        repunit = repunit * (seq_t)10 + (seq_t)1;   /* next repunit: 111...1 */
        sum += repunit;                           /* accumulate */
    }

    return (seq_t)d * sum;                      /* final term */
}
