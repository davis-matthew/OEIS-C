/*
 * A249693  a(4n) = 3*n+1, a(2n+1) = 3*n+2, a(4n+2) = 3*n.
 * https://oeis.org/A249693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(4n) = 3*n + 1, a(2n+1) = 3*n + 2, a(4n+2) = 3*n
 * Range:     u8 n <= 170, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {          /* even n */
        if ((n & 3) == 0) {      /* n % 4 == 0  →  a = 3*(n/4) + 1 */
            idx_t k = n >> 2;    /* k = n / 4 (floor) */
            return (seq_t)(3 * k + 1);
        } else {                 /* n % 4 == 2  →  a = 3*(n/4)       */
            idx_t k = n >> 2;    /* k = n / 4 (floor) */
            return (seq_t)(3 * k);
        }
    } else {                     /* odd n  →  a = 3*((n-1)/2) + 2 */
        idx_t k = (n - 1) >> 1;  /* k = (n-1)/2 (floor) */
        return (seq_t)(3 * k + 2);
    }
}
