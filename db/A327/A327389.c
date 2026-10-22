/*
 * A327389  Maximum divisor of n that is prime or whose prime indices are
 *          pairwise coprime.
 * https://oeis.org/A327389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximum divisor d | n such that d is prime or the indices
 *            of the primes dividing d are pairwise coprime. Indexing follows
 *            OEIS starting at a(1)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   build_primes_upto, build_spf, gcd_int, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/build_spf.h"
#include "../utils/build_primes_upto.h"
#include "../utils/gcd_int.h"
#include "../utils/pow2.h"

/* Cached data for all n seen */
static idx_t cached_max_n = 0;
static uint64_t *cached_spf = NULL;
static uint64_t *cached_primes = NULL;
static size_t cached_prime_cnt = 0;
static uint32_t *cached_prime_index = NULL;

/* Ensure the cache covers at least 1 .. n */
static void ensure_cache(idx_t n) {
    if (n <= cached_max_n) return;

    free(cached_spf);
    free(cached_primes);
    free(cached_prime_index);

    /* Smallest prime factor table up to n */
    build_spf(n, &cached_spf);

    /* Pointers to primes up to n */
    build_primes_upto(n, &cached_primes, &cached_prime_cnt);

    /* Map prime value → 1‑based index in the primes list */
    cached_prime_index = (uint32_t *)calloc((size_t)(n + 1), sizeof(uint32_t));
    for (size_t i = 0; i < cached_prime_cnt; ++i) {
        uint64_t p = cached_primes[i];
        cached_prime_index[(size_t)p] = (uint32_t)(i + 1);
    }

    cached_max_n = n;
}

/* Main sequence generator */
seq_t a(idx_t n) {
    if (n == 0) return 0;         /* undefined for n < 1, return 0 */

    ensure_cache(n);

    uint64_t tmp = n;
    /* Exponent of 2 in n (if any) */
    uint64_t pow2exp = 0;
    /* List of distinct odd primes dividing n (up to about 5 for n ≤ 10000) */
    uint64_t primes[8];
    uint32_t indices[8];
    size_t prime_cnt = 0;

    while (tmp > 1) {
        uint64_t p = cached_spf[tmp];
        uint64_t e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }
        if (p == 2) {
            pow2exp = e;
        } else {
            primes[prime_cnt] = p;
            indices[prime_cnt] = cached_prime_index[(size_t)p];
            ++prime_cnt;
        }
    }

    /* Base factor 2^pow2exp, if present */
    seq_t base = (pow2exp > 0) ? pow2(pow2exp) : (seq_t)1;

    /* Iterate over all subsets of the odd prime factors */
    seq_t best = (seq_t)1;
    size_t max_mask = ((size_t)1 << prime_cnt);
    for (size_t mask = 0; mask < max_mask; ++mask) {
        bool ok = true;
        /* Check pairwise coprime of indices in this subset */
        for (size_t i = 0; i < prime_cnt && ok; ++i) {
            if (mask & ((size_t)1 << i)) {
                for (size_t j = i + 1; j < prime_cnt && ok; ++j) {
                    if (mask & ((size_t)1 << j)) {
                        if (gcd_int((int)indices[i], (int)indices[j]) != 1)
                            ok = false;
                    }
                }
            }
        }
        if (!ok) continue;

        /* Compute product for this subset */
        seq_t prod = base;
        for (size_t i = 0; i < prime_cnt; ++i) {
            if (mask & ((size_t)1 << i))
                prod *= (seq_t)primes[i];
        }
        if (prod > best) best = prod;
    }

    return best;
}
