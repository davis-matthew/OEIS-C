/*
 * A355453  a(n) = 1 if the third smallest divisor of n is not a prime,
 *          otherwise 0.
 * https://oeis.org/A355453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the third smallest divisor of n is not prime,
 *            otherwise 0
 * Range:     u8 n <= 9515, u16 n <= 9515, u32 n <= 9515, u64 n <= 9515,
 *            u128 n <= 9515, big any n
 * Helpers:   _smallest_factor, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We need to test primality; the helper function is available in the
   utils library. */
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/_smallest_factor.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 0;  /* n = 1 has fewer than 3 divisors */

    uint64_t N = (uint64_t)n;
    uint64_t spf = _smallest_factor(N);

    if (spf == N) return 0;   /* prime – only two divisors */

    uint64_t m = N;
    unsigned e1 = 0;
    while ((m % spf) == 0) {
        m /= spf;
        ++e1;
    }

    /* Determine the third divisor */
    uint64_t third;
    if (e1 == 1) {               /* only one power of the smallest prime */
        if (m == 1) return 0;    /* should not happen (spf != N handled) */
        uint64_t p2 = _smallest_factor(m);   /* next distinct prime factor */
        third = p2;
    } else {                     /* e1 >= 2 */
        uint64_t spf_sq = spf * spf;
        if (m == 1) {            /* n = spf^e1, no second prime */
            third = spf_sq;
        } else {                 /* there is a second prime factor */
            uint64_t p2 = _smallest_factor(m);
            third = spf_sq < p2 ? spf_sq : p2;
        }
    }

    /* Return 1 if third divisor is not prime, else 0 */
    return (seq_t)(is_prime(third) ? 0 : 1);
}
