/*
 * A263390  a(3n) = n, otherwise a(n) = a(floor(2n/3)).
 * https://oeis.org/A263390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3n) = n; otherwise a(n) = a(floor(2n/3))
 * Range:     u8 n <= 767, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Iterate until n is a multiple of 3, using the recurrence
       a(n) = a(floor(2n/3)).  The expression n - (n+2)/3 equals
       floor(2n/3) and avoids overflow on the product 2*n. */
    while (n % 3u != 0) {
        n = n - (n + 2u) / 3u;   /* floor(2*n/3) */
    }
    return (seq_t)(n / 3u);
}
