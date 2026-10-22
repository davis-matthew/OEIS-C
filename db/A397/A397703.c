/*
 * A397703  Numbers k such that sigma(k) = psi(k) + tau(k)^4 + omega(k)^4.
 * https://oeis.org/A397703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 241149 * p^2 where p is the nth prime not equal to 3, 31,
 *            or 2593
 * Range:     u8 none, u16 none, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/build_primes_upto.h"

/* compute the n-th sequence value */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset is 1 – n=0 never requested */

    /* constants */
    const seq_t FACTOR = 241149U;

    /* static state: primes array and its size */
    static uint64_t *primes = NULL;
    static size_t prime_count = 0;
    static size_t limit_used = 0;

    /* estimate an upper bound on the n-th (valid) prime
       The n-th prime (n>=6) is < n*(log n + log log n),
       we add a small safety margin.   */
    size_t estimate;
    if (n < 6) {
        estimate = 15;
    } else {
        double dn = (double)n;
        estimate = (size_t)(dn * (log(dn) + log(log(dn)))) + 3;
    }
    /* ensure we cover the excluded primes */
    if (estimate < 2600) estimate = 2600;

    /* we may need to grow the prime list if the current limit is too small */
    while (1) {
        if (!primes || estimate > limit_used) {
            /* rebuild the list up to the new limit */
            if (primes) free(primes);
            build_primes_upto(estimate, &primes, &prime_count);
            limit_used = estimate;
        }

        /* scan the list, skipping the three forbidden primes */
        size_t idx = 0;
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = primes[i];
            if (p == 3 || p == 31 || p == 2593) continue;
            ++idx;
            if (idx == n) {
                seq_t p2 = (seq_t)p * (seq_t)p;
                seq_t result = FACTOR;
                result *= p2;
                return result;
            }
        }

        /* if we didn't find the n-th term, increase the bound and retry */
        estimate *= 2;
    }
}
