/*
 * A374580  a(n) is the numerator of (120*n^2 + 151*n + 47)/(512*n^4 +
 *          1024*n^3 + 712*n^2 + 194*n + 15).
 * https://oeis.org/A374580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 3119, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerator of the reduced fraction
 * (120*n^2 + 151*n + 47) /
 * (512*n^4 + 1024*n^3 + 712*n^2 + 194*n + 15)
 * after dividing numerator and denominator by their gcd.
 * This yields the OEIS A374580 sequence.
 */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;          /* cast idx_t to seq_t */
    seq_t n2 = sn * sn;           /* n^2 */
    seq_t n3 = n2 * sn;           /* n^3 */
    seq_t n4 = n3 * sn;           /* n^4 */

    /* Unsimplified numerator and denominator */
    seq_t num = (seq_t)120 * n2 + (seq_t)151 * sn + (seq_t)47;
    seq_t den = (seq_t)512 * n4 + (seq_t)1024 * n3 +
                (seq_t)712 * n2 + (seq_t)194 * sn + (seq_t)15;

    /* Reduce fraction */
    seq_t g = gcd(num, den);
    return num / g;
}
