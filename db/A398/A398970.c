/*
 * A398970  Weak Carmichael numbers of the form (6*m+1)*(12*m+1)*(18*m+1)
 *          where exactly one linear factor is composite.
 * https://oeis.org/A398970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 17, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A398970: Weak Carmichael numbers of the form (6m+1)(12m+1)(18m+1)
   where exactly one of the three linear factors is composite.
   The function generates terms on demand and caches previously found
   values. */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t *cache = NULL;          /* array of found numbers          */
static size_t cache_cap = 0;         /* allocated capacity              */
static size_t cache_len = 0;         /* number of filled elements       */
static idx_t m_next = 1;             /* next m to test                  */

/* Ensure the cache has at least 'needed' slots. */
static void grow_cache(size_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        if (new_cap < needed) new_cap = needed;
        seq_t *new_buf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_buf;
        cache_cap = new_cap;
    }
}

/* Append a prime to a list if it is not already present. */
static void add_prime(uint64_t p, uint64_t *list, size_t *cnt)
{
    for (size_t i = 0; i < *cnt; ++i) {
        if (list[i] == p) return;
    }
    list[(*cnt)++] = p;
}

/* Compute the n-th term of A398970. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined, return 0 safety */

    while (cache_len < n) {
        idx_t m = m_next++;

        uint64_t a1 = 6 * m + 1;
        uint64_t a2 = 12 * m + 1;
        uint64_t a3 = 18 * m + 1;

        /* Count how many of the three factors are composite. */
        int comp_cnt = 0;
        if (!is_prime((idx_t)a1)) ++comp_cnt;
        if (!is_prime((idx_t)a2)) ++comp_cnt;
        if (!is_prime((idx_t)a3)) ++comp_cnt;
        if (comp_cnt != 1) continue;   /* not a candidate */

        /* Gather distinct prime divisors of the product. */
        uint64_t primes[16];
        size_t prime_cnt = 0;

        uint64_t factors[3] = { a1, a2, a3 };
        for (int k = 0; k < 3; ++k) {
            uint64_t x = factors[k];
            if (is_prime((idx_t)x)) {
                add_prime(x, primes, &prime_cnt);
                continue;
            }
            uint64_t tmp = x;
            for (uint64_t d = 2; d * d <= tmp; ++d) {
                if (tmp % d == 0) {
                    add_prime(d, primes, &prime_cnt);
                    while (tmp % d == 0) tmp /= d;
                }
            }
            if (tmp > 1) {
                add_prime(tmp, primes, &prime_cnt);
            }
        }

        /* Compute the product. */
        seq_t prod = (seq_t)a1;
        prod *= (seq_t)a2;
        prod *= (seq_t)a3;
        seq_t one_less = prod - (seq_t)1;

        /* Verify weak Carmichael condition: (p-1) | (N-1) for each prime p. */
        bool ok = true;
        for (size_t i = 0; i < prime_cnt && ok; ++i) {
            seq_t mod_base = (seq_t)(primes[i] - 1);
            if (one_less % mod_base != (seq_t)0) ok = false;
        }

        if (ok) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = prod;
        }
    }

    return cache[n - 1];
}
