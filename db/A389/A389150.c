/*
 * A389150  Nonprimes k with no proper divisor d such that gcd(d^2 - 1, k) =
 *          1.
 * https://oeis.org/A389150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = Nonprimes k with no proper divisor d such that gcd(d^2 -
 *            1, k) = 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"

/* Cached sequence values */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t last_candidate = 1;

seq_t a(idx_t n)
{
    /* Return cached value if already computed */
    if (cache_len >= n) {
        return cache[n - 1];
    }

    /* Initialize cache on first call */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
        last_candidate = 1;
    }

    /* Ensure the first term 1 is stored */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* Generate sequence terms until we have n entries */
    while (cache_len < (size_t)n) {
        ++last_candidate;
        uint64_t k = last_candidate;

        /* Skip numbers not divisible by 6 (except the term 1) */
        if (k > 1 && k % 6 != 0)
            continue;

        /* Factorise k */
        uint64_t primes[64];
        uint64_t exps[64];
        size_t pcnt = 0;
        factor_u64(k, primes, exps, &pcnt);

        /* Reject if k is not squarefree */
        bool squarefree = true;
        for (size_t i = 0; i < pcnt; ++i) {
            if (exps[i] > 1) {
                squarefree = false;
                break;
            }
        }
        if (!squarefree)
            continue;

        /* Check the defining property */
        bool good = true;
        if (k > 1 && pcnt > 0) {
            uint64_t fullmask = (pcnt >= 64) ? ~0ULL : ((1ULL << pcnt) - 1);
            for (uint64_t mask = 1; mask < fullmask; ++mask) {
                uint64_t d = 1;
                for (size_t b = 0; b < pcnt; ++b) {
                    if (mask & (1ULL << b))
                        d *= primes[b];
                }

                seq_t d_minus = (seq_t)(d - 1);
                seq_t d_plus  = (seq_t)(d + 1);
                seq_t k_seq   = (seq_t)k;

                seq_t g1 = gcd(d_minus, k_seq);
                if (g1 > (seq_t)1)
                    continue;

                seq_t g2 = gcd(d_plus, k_seq);
                if (g2 > (seq_t)1)
                    continue;

                /* Found a proper divisor with gcd 1 */
                good = false;
                break;
            }
        }

        if (good) {
            /* Grow cache if necessary */
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                seq_t *tmp = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) abort();  /* allocation failure */
                cache = tmp;
            }
            cache[cache_len++] = (seq_t)k;
        }
    }

    return cache[n - 1];
}
