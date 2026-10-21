/*
 * A243466  Number of ways 4 domicules can be placed on an n X n square.
 * https://oeis.org/A243466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-104802 +61647*n +50017*n^2 -35304*n^3 -4984*n^4 *
 *            +6480*n^5 -448*n^6 -384*n^7 +64*n^8)/6 for n>=4 * a(3) = 58,
 *            a(n)=0 for n<3 * Implements the polynomial directly with 128‑bit
 *            integer arithmetic * for intermediate values, then converts the
 *            result to seq_t. * The code works for any seq_t...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 12, u64 n <= 191, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Small indices */
    if (n < 3) return (seq_t)0;
    if (n == 3) return (seq_t)58;

    /* Use signed 128‑bit arithmetic for intermediate values */
    long long n64 = (long long)n;          /* n is small enough for 64‑bit */
    __int128 num = 0;

    /* Add all terms of the numerator */
    num += (__int128)(-104802);
    num += (__int128)61647 * n64;
    num += (__int128)50017 * n64 * n64;
    num += (__int128)(-35304) * n64 * n64 * n64;
    num += (__int128)(-4984)  * n64 * n64 * n64 * n64;
    num += (__int128)6480     * n64 * n64 * n64 * n64 * n64;
    num += (__int128)(-448)   * n64 * n64 * n64 * n64 * n64 * n64;
    num += (__int128)(-384)   * n64 * n64 * n64 * n64 * n64 * n64 * n64;
    num += (__int128)64       * n64 * n64 * n64 * n64 * n64 * n64 * n64 * n64;

    /* Divide by 6 to get the final value */
    num /= 6;

    return (seq_t)num;
}
