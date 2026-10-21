/*
 * A285690  a(1) = 2; a(n + 1) = smallest prime > a(n) such that a(n + 1) -
 *          a(n) is the product of five primes.
 * https://oeis.org/A285690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 690, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A285690: a(1)=2; a(n+1)=smallest prime > a(n)
   such that a(n+1)-a(n) is the product of five (not necessarily
   distinct) primes. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

static void ensure_cache(idx_t n)
{
    if ((idx_t)cache_cap >= n)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < (size_t)n)
        new_cap *= 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) /* undefined but return 0 for safety */ return 0;

    ensure_cache(n);

    /* Initialise the sequence with a(1)=2 if not already done */
    if (cache_len == 0) {
        cache[0] = (seq_t)2;
        cache_len = 1;
    }

    /* Generate terms up to n if they are not yet cached */
    if (cache_len < (size_t)n) {
        uint64_t prev = (uint64_t)cache[cache_len - 1];

        for (size_t i = cache_len; i < (size_t)n; ++i) {
            /* Find the next prime > prev that satisfies the
               product‑of‑five‑primes condition on the difference. */
            uint64_t cand = next_prime(prev);
            while (1) {
                uint64_t diff = cand - prev;

                uint64_t pf[64], exp[64];
                size_t cnt;
                factor_u64(diff, pf, exp, &cnt);

                int omega = 0;
                for (size_t j = 0; j < cnt; ++j)
                    omega += (int)exp[j];

                if (omega == 5) {
                    cache[i] = (seq_t)cand;
                    cache_len = i + 1;
                    prev = cand;
                    break;
                }
                cand = next_prime(cand);
            }
        }
    }

    return cache[n - 1];
}
