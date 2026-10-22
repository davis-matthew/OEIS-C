/*
 * A337349  To get a(n), take 3*n+1 and divide out any power of 2; then
 *          multiply by 3, subtract 1 and divide out any power of 2.
 * https://oeis.org/A337349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = odd_part(3*odd_part(3*n+1)-1)
 * Range:     u8 n <= 29, u16 n <= 9999, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    /* First step: 3*n + 1, then remove all factors of 2 */
    x = 3 * x + 1;
    while ((x & 1) == 0)
        x >>= 1;

    /* Second step: 3*prev - 1, then remove all factors of 2 */
    x = 3 * x - 1;
    while ((x & 1) == 0)
        x >>= 1;

    return x;
}
