/*
 * A359549  a(n) = 1 if n is either an odd squarefree number squared or twice
 *          such a number, otherwise 0.
 * https://oeis.org/A359549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 1 if n is either the square of an odd squarefree number *
 *            or twice such a square; otherwise 0
 * Range:     u8 n <= 8778, u16 n <= 8778, u32 n <= 8778, u64 n <= 8778,
 *            u128 n <= 8778, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    idx_t m = n;
    /* count exponent of 2 */
    int e2 = 0;
    while ((m & 1ULL) == 0) {
        e2++;
        m >>= 1;
    }
    if (e2 > 1) return 0;          /* exponent of 2 must be 0 or 1 */

    /* m is now odd.  Factor odd primes, each exponent must be exactly 2. */
    idx_t p = 3;
    while (p <= m / p) {           /* avoid overflow */
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }
            if (exp != 2) return 0; /* odd primes must appear squared */
        }
        p += 2;
    }
    if (m > 1) return 0;           /* remaining factor has exponent 1 */

    return 1;
}
