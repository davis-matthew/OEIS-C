/*
 * A345068  a(n) = Sum_{d|n, d>1} d^floor(1/omega(d)).
 * https://oeis.org/A345068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Sum_{d|n, d>1} d^⌊1/ω(d)⌋ , where ω(d) is the number of
 *            distinct prime divisors of d
 * Range:     u8 n <= 242, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;          /* a(1) = 0 */

    seq_t sum = 0;
    idx_t nn = n;

    for (idx_t i = 1; i <= nn / i; ++i) {
        if (nn % i == 0) {
            /* divisor i */
            idx_t d1 = i;
            if (d1 > 1) {
                int omega = count_distinct_primes((uint64_t)d1);
                if (omega == 1)
                    sum += (seq_t)d1;   /* prime power: add d */
                else
                    sum += (seq_t)1;    /* composite with ≥2 primes: add 1 */
            }

            /* paired divisor nn / i, avoid double‑counting when i*i==nn */
            idx_t d2 = nn / i;
            if (d2 != d1 && d2 > 1) {
                int omega = count_distinct_primes((uint64_t)d2);
                if (omega == 1)
                    sum += (seq_t)d2;
                else
                    sum += (seq_t)1;
            }
        }
    }

    return sum;
}
