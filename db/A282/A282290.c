/*
 * A282290  Expansion of (Sum_{p prime, i>=2} x^(p^i))*(Sum_{j>=2}
 *          mu(j)^2*x^j), where mu() is the Moebius function (A008683).
 * https://oeis.org/A282290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = #{(p^k , q): p prime, k≥2, q ≥ 2, p^k + q = n, * q
 *            square‑free} * Sequence A282290: expansion of (Σ_{p prime, i≥2}
 *            x^(p^i)) * * (Σ_{j≥2} μ(j)^2 x^j) * * Implementation uses a
 *            sieve of primes up to √n to generate all * proper prime powers ≤
 *            n−2, then counts the number of square‑free * com...
 * Range:     u8 n <= 110, u16 n <= 110, u32 n <= 110, u64 n <= 110,
 *            u128 n <= 110, big any n
 * Helpers:   build_primes_upto, int_sqrt, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/build_primes_upto.h"   /* build_primes_upto() */
#include "../utils/int_sqrt.h"           /* int_sqrt()     */
#include "../utils/is_squarefree.h"       /* is_squarefree() */

seq_t a(idx_t n)
{
    if (n < 4)               /* smallest proper prime power is 2^2 = 4 */
        return 0;

    idx_t limit = n - 2;          /* largest allowed prime power */

    /* primes needed to produce p^2 ≤ n → p ≤ √n */
    idx_t sqrt_n = int_sqrt(n);
    uint64_t *primes = NULL;
    size_t primes_cnt = 0;
    build_primes_upto(sqrt_n, &primes, &primes_cnt);

    seq_t count = 0;

    for (size_t p_idx = 0; p_idx < primes_cnt; ++p_idx) {
        uint64_t p = primes[p_idx];
        idx_t power = (idx_t)p * (idx_t)p;    /* start with p^2 */

        while (power <= limit) {
            idx_t j = n - power;              /* candidate square‑free part */
            if (j >= 2 && is_squarefree((seq_t)j))
                ++count;

            /* advance to next power, guarding against overflow */
            if (power > limit / p)
                break;
            power *= p;
        }
    }

    free(primes);          /* memory allocated by build_primes_upto() */
    return count;
}
