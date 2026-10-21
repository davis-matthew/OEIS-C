/*
 * A272058  Start with all terms set to 0. Then add n to the next n+3 terms
 *          for n=0,1,2,... .
 * https://oeis.org/A272058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n^2 + 6*n - 23 + (7 - 2*n)*(-1)^n)/16 for n > 1 a(0) =
 *            0, a(1) = 0
 * Range:     u8 n <= 25, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 2) {
        return 0;
    }
#if defined(__SIZEOF_INT128__)
    __int128 nn = n;
#else
    long long nn = n;
#endif
    /* numerator: 6*n^2 + 6*n - 23 + (7 - 2*n)*(-1)^n */
    __int128 part1 = ( __int128)6 * nn * nn + ( __int128)6 * nn - 23;
    __int128 sign = (n & 1) ? -1 : 1;
    __int128 part2 = (7 - 2 * nn) * sign;
    __int128 numerator = part1 + part2;
    __int128 result = numerator / 16;   /* integer division */
    return (seq_t)result;
}
