/*
 * A258978  a(n) = 1 + sigma(n) + sigma(n)^2 + sigma(n)^3 + sigma(n)^4.
 * https://oeis.org/A258978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + sigma(n) + sigma(n)^2 + sigma(n)^3 + sigma(n)^4
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 107, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0; /* undefined for n < 1, return 0 */

    seq_t s  = sigma_of(n);      /* sigma(n)          */
    seq_t s2 = s * s;            /* sigma(n)^2        */
    seq_t s3 = s2 * s;           /* sigma(n)^3        */
    seq_t s4 = s3 * s;           /* sigma(n)^4        */

    return (seq_t)1 + s + s2 + s3 + s4;
}
