/*
 * A385096  Numbers such that (-1)^Omega(k)*Moebius(espf(k)) < 0, where
 *          Moebius = A008683 and Omega = A001221, and espf(k) is the exponent
 *          of the smallest prime factor of k, with espf(1) = 1.
 * https://oeis.org/A385096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   _smallest_prime_factor, mu_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A385096: a(n) = the n-th integer k such that (-1)^Ω(k)*µ(espf(k)) < 0,
   where Ω is the number of distinct prime factors of k (A001221) and
   espf(k) is the exponent of the smallest prime factor of k. The terms
   are produced by testing consecutive integers and caching those that
   satisfy the condition. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/_smallest_prime_factor.h"
#include "../utils/mu_of.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for n < 1 */

    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t last_checked = 1; /* start from 1, skip it */

    while (cache_len < (size_t)n) {
        ++last_checked;
        uint64_t k = (uint64_t)last_checked;

        /* compute exponent of smallest prime factor and parity of distinct primes */
        uint64_t first_spf = 0;
        uint64_t e = 0;
        uint64_t temp = k;
        uint64_t prev_spf = 0;
        int distinct_parity = 0; /* 0 for even, 1 for odd */

        while (temp > 1) {
            uint64_t p = _smallest_prime_factor(temp);

            if (first_spf == 0) {
                first_spf = p;
                /* exponent of p in k */
                uint64_t t = temp;
                while (t % p == 0) {
                    ++e;
                    t /= p;
                }
            }

            if (p != prev_spf) {
                distinct_parity ^= 1;
                prev_spf = p;
            }
            while (temp % p == 0) {
                temp /= p;
            }
        }

        int mu = mu_of((idx_t)e);
        if (mu != 0) {
            int sign_distinct = (distinct_parity == 0) ? 1 : -1;
            if ((sign_distinct * mu) < 0) {
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                    seq_t *ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!ptr) abort();
                    cache = ptr;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)k;
            }
        }
    }

    return cache[n - 1];
}
