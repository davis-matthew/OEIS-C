/*
 * A294876  a(n) = Product_{d|n, d>1} prime(gcd(d,n/d)).
 * https://oeis.org/A294876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{d|n , d>1} prime(gcd(d , n/d)) (OEIS A294876)
 * Range:     u8 n <= 23, u16 n <= 71, u32 n <= 143, u64 n <= 719,
 *            u128 n <= 3599, big any n
 * Helpers:   gcd, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 1)            /* a(1) = 1 */
        return (seq_t)1;

    seq_t prod = (seq_t)1;

    /* Enumerate divisors d from 2 up to sqrt(n) without overflow */
    for (idx_t d = 2; d <= n / d; ++d) {
        if (n % d != 0)
            continue;

        idx_t e = n / d;                         /* complementary divisor */

        /* gcd(d, e) fits in idx_t because both ≤ n */
        seq_t g = gcd((seq_t)d, (seq_t)e);
        idx_t k = (idx_t)g;                      /* safe cast, g ≤ n ≤ UINT64_MAX */

        seq_t p = (seq_t)get_prime(k);           /* prime(k) */

        prod *= p;                               /* divisor d */

        if (e != d)                              /* divisor e (if distinct) */
            prod *= p;
    }

    /* Handle the divisor d = n (paired with 1): gcd(n,1) = 1, prime(1)=2 */
    prod *= (seq_t)2;

    return prod;
}
