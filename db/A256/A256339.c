/*
 * A256339  Distinct-digit primes that are concatenation of prime(m) and m for
 *          some m.
 * https://oeis.org/A256339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = distinct‑digit prime obtained by concatenating prime(m)
 *            and m for some m. The function builds the terms by search and
 *            caches them, so that any n ≥ 1 can be returned in O(n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 4, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   concat_parts, digit_counts, get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/get_prime.h"
#include "../utils/concat_parts.h"
#include "../utils/is_prime_big.h"
#include "../utils/digit_counts.h"

/* Cached results: cache[i] == a(i+1) */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
/* Next m to test (the m corresponding to prime(m)) */
static idx_t cur_m = 1;

seq_t a(idx_t n)
{
    /* Generate terms until we have at least n of them */
    while (cache_len < n)
    {
        /* m-th prime (p) and candidate = concat(p, m) */
        uint32_t p = get_prime(cur_m);
        seq_t candidate = concat_parts((idx_t)p, cur_m);

        /* Test primality and distinct digits */
        if (is_prime_big(candidate))
        {
            unsigned char cnt[10] = {0};
            digit_counts(candidate, cnt);

            bool distinct = true;
            for (int d = 0; d < 10; ++d)
            {
                if (cnt[d] > 1)
                {
                    distinct = false;
                    break;
                }
            }

            if (distinct)
            {
                /* Grow cache if necessary */
                if (cache_len + 1 > cache_cap)
                {
                    size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
                    while (new_cap <= cache_len + 1)
                        new_cap *= 2;
                    cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                    cache_cap = new_cap;
                }
                cache[cache_len++] = candidate;
            }
        }
        ++cur_m;
    }
    return cache[n - 1];
}
