/*
 * A377281  Difference between the n-th prime and the next prime-power
 *          (exclusive).
 * https://oeis.org/A377281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A345531(n) - prime(n) (next prime power > prime(n) minus
 *            the prime)
 * Range:     u8 n <= 31, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   get_prime, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* obtain the n-th prime and the (n+1)-th prime (next prime) */
    uint32_t p0 = get_prime(n);          /* nth prime */
    uint32_t p1 = get_prime(n + 1);      /* next prime */

    /* start with the difference to the next prime */
    seq_t best = (seq_t)p1 - (seq_t)p0;

    /* determine the largest exponent e such that 2^e > p0 */
    int e_max = 1;                       /* e such that 2^e > p0 */
    for (seq_t pow = 2; pow <= (seq_t)p0; pow <<= 1)
        ++e_max;

    /* search for composite prime powers that are closer */
    for (int e = 2; e <= e_max; ++e) {
        /* iterate over prime bases q and find smallest q^e > p0 */
        for (uint64_t q = 2; ; ++q) {
            if (!is_prime_small(q))
                continue;

            /* compute q^e (loop instead of pow to stay in integer domain) */
            seq_t val = 1;
            for (int i = 0; i < e; ++i) {
                val *= (seq_t)q;
                if (val > (seq_t)p0 && val - (seq_t)p0 < best)
                    break;
            }
            if (val > (seq_t)p0) {
                seq_t diff = val - (seq_t)p0;
                if (diff < best)
                    best = diff;
                break;               /* larger primes yield larger val */
            }
        }
    }

    return best;
}
