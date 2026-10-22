/*
 * A307559  a(n) = floor(n/3)*(n - floor(n/3))*(n - floor(n/3) - 1).
 * https://oeis.org/A307559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/3)*(n - floor(n/3))*(n-floor(n/3)-1)
 * Range:     u8 n <= 12, u16 n <= 76, u32 n <= 3072, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute floor(n/3) as an unsigned integer */
    seq_t m = (seq_t)(n / 3u);

    /* Promote n to seq_t for the remaining arithmetic */
    seq_t sn = n;

    /* Compute n - m */
    seq_t x = sn - m;

    /* a(n) = m * (n - m) * (n - m - 1) */
    return m * x * (x - 1);
}
