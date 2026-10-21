/*
 * A286474  Compound filter: a(n) = 4*A032742(n) + (n mod 4), a(1) = 1.
 * https://oeis.org/A286474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n==1 else 4*(n / spf(n)) + (n mod 4)
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  where spf(n) is the smallest prime factor of n. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;

    /* Find the smallest prime factor of n (spf).  If no factor is found,
       n itself is prime and spf = n. */
    idx_t spf = n;
    idx_t d = 2;
    while (d <= n / d) {          /* avoid overflow in d*d */
        if (n % d == 0) {
            spf = d;
            break;
        }
        ++d;
    }

    /* largest proper divisor = n / spf */
    seq_t largest = (seq_t)(n / spf);

    /* a(n) = 4 * largest + (n mod 4) */
    seq_t result = (seq_t)4 * largest + (seq_t)(n & 3);
    return result;
}
