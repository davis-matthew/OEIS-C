/*
 * A328202  a(n) is the greatest common divisor of all the numbers in row n of
 *          Pascal's triangle excluding 1 and n.
 * https://oeis.org/A328202
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, look, changed
 * Formula:   a(n) = GCD of binomial(n,k) for k = 2 … n‑2 (the interior
 *            entries of row n of Pascal's triangle)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, v_p_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/v_p_uint.h"

seq_t a(idx_t n)
{
    if (n < 4)
        return (seq_t)0;        /* undefined for n < 4, unused in tests */

    /* Obtain all primes ≤ n. */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto((uint64_t)n, &primes, &prime_cnt);

    seq_t result = (seq_t)1;

    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        uint64_t p = primes[pi];

        /* Exponent of p in nC1 = n. */
        unsigned int vp_prev = v_p_uint((uint64_t)n, p);

        unsigned int min_exp = UINT_MAX;

        /* Iterate over k = 2 … n‑2.
           Using the recurrence:
           v_p(nCk) = v_p(nC(k‑1)) + v_p(n‑(k‑1)) – v_p(k). */
        for (idx_t k = 2; k <= n - 2; ++k) {
            unsigned int vp_nk_1 = v_p_uint((uint64_t)(n - (k - 1)), p); /* v_p(n‑(k‑1)) */
            unsigned int vp_k     = v_p_uint((uint64_t)k, p);             /* v_p(k) */
            unsigned int vp_k_val = vp_prev + vp_nk_1 - vp_k;             /* v_p(nCk) */

            if (vp_k_val < min_exp)
                min_exp = vp_k_val;

            vp_prev = vp_k_val;
        }

        if (min_exp > 0) {
            /* Multiply result by p^min_exp. */
            for (unsigned int e = 0; e < min_exp; ++e)
                result *= (seq_t)p;
        }
    }

    free(primes);
    return result;
}
