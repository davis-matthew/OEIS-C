/*
 * A381431  Heinz number of the section-sum partition of the prime indices of
 *          n.
 * https://oeis.org/A381431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   build_primes_upto, get_prime, int_sqrt, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the Heinz number of the section-sum partition of the prime indices of n. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/get_prime.h"
#include "../utils/int_sqrt.h"
#include "../utils/build_primes_upto.h"
#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    /* Special case n = 1 */
    if (n == 1)
        return (seq_t)1;

    /* Dynamically maintain a list of primes up to sqrt(n) */
    static uint64_t *prime_list = NULL;
    static size_t prime_count = 0;
    static idx_t prime_limit = 0;

    /* Compute integer sqrt(n) */
    idx_t sqrt_n = int_sqrt(n);

    if (prime_limit < sqrt_n) {
        uint64_t *new_primes = NULL;
        size_t new_count = 0;
        build_primes_upto(sqrt_n, &new_primes, &new_count);

        free(prime_list);
        prime_list = new_primes;
        prime_count = new_count;
        prime_limit = sqrt_n;
    }

    /* Factor n: collect distinct prime indices and exponents */
    uint64_t m = (uint64_t)n;
    struct { idx_t idx; idx_t exp; } fac[64];
    size_t fac_cnt = 0;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > m)
            break;
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            fac[fac_cnt].idx = (idx_t)(i + 1);   /* prime index is 1‑based */
            fac[fac_cnt].exp = e;
            ++fac_cnt;
        }
    }
    if (m > 1) { /* remaining factor is prime */
        idx_t idx_m = (idx_t)prime_pi(m); /* index of the remaining prime */
        fac[fac_cnt].idx  = idx_m;
        fac[fac_cnt].exp = 1;
        ++fac_cnt;
    }

    /* Determine the maximum exponent */
    idx_t max_exp = 0;
    for (size_t i = 0; i < fac_cnt; ++i)
        if (fac[i].exp > max_exp)
            max_exp = fac[i].exp;

    /* Compute product of primes corresponding to each section-sum value */
    seq_t product = (seq_t)1;
    for (idx_t k = 1; k <= max_exp; ++k) {
        idx_t sum_indices = 0;
        for (size_t i = 0; i < fac_cnt; ++i)
            if (fac[i].exp >= k)
                sum_indices += fac[i].idx;

        /* sum_indices should be >= 1 */
        uint32_t prime_of_sum = get_prime(sum_indices);
        product *= (seq_t)prime_of_sum;
    }

    return product;
}
