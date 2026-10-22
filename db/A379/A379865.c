/*
 * A379865  Number of base 10 digits of 2^(p-1)*(2^p-1) where p = prime(n).
 * https://oeis.org/A379865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = number of decimal digits of 2^(p-1)*(2^p-1) where p =
 *            prime(n)
 * Range:     u8 n <= 82, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* nth prime */
    uint32_t p = get_prime(n);

    /* log10(2) as long double for high accuracy */
    const long double LOG10_2 = 0.3010299956639811952137388947244930L;

    /* log10( N ) = (2p-1)*log10(2) + log10(1-2^-p) */
    long double log_val = (long double)(2ULL * p - 1ULL) * LOG10_2;

    /* Small correction: log10(1-2^-p). For very large p the term is zero
       in long double precision, which is fine. */
    if (p < 1024U) {
        long double inv = powl(2.0L, -(long double)p);   /* 2^-p */
        log_val += log10l(1.0L - inv);
    }

    /* Number of decimal digits = floor(log10(N)) + 1 */
    uint64_t digits = (uint64_t)floor(log_val) + 1ULL;
    return (seq_t)digits;
}
