/*
 * A306266  Number of reciprocally monophyletic coalescence sequences for 2n
 *          lineages, n each in 2 species.
 * https://oeis.org/A306266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n!)^2 * (2*n-2)! / 2^(2*n-2)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   build_primes_upto, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0; /* undefined for offset 1, but safe fallback */
    }

    idx_t limit = 2 * n - 2;            /* (2*n-2) */
    uint64_t *primes = NULL;            /* array of primes <= limit */
    size_t primes_cnt = 0;

    build_primes_upto(limit, &primes, &primes_cnt);

    seq_t result = 1;

    for (size_t idx = 0; idx < primes_cnt; ++idx) {
        uint64_t p = primes[idx];
        idx_t exp = 0;

        /* exponent from (n!)^2   */
        idx_t m = n;
        while (m) {
            m /= p;
            exp += 2 * m;
        }

        /* exponent from (2n-2)! */
        m = limit;
        while (m) {
            m /= p;
            exp += m;
        }

        /* subtract the power of 2 in the denominator */
        if (p == 2) {
            exp -= limit; /* limit == 2*n-2 */
        }

        if (exp > 0) {
            seq_t pow_p = seq_pow((seq_t)p, exp);
            result = result * pow_p;
        }
    }

    free(primes);
    return result;
}
