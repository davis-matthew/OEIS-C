/*
 * A364611  For p = 5 and n > 0, write n = p^m + k, m >= 0, with maximal p^m
 *          <= n, with 0 <= k < p^(m+1) - p^m, then for n such that k=0,
 *          a(n)=n, and for n such that k > 0, a(n) is the smallest q*a(k),
 *          prime q != p, that is not already a term.
 * https://oeis.org/A364611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 351, u32 n <= 6075, u64 n <= 9977,
 *            u128 n <= 9977, big any n
 * Helpers:   build_primes_upto, cache_has
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A364611 (D(5)).  For n > 0 write n = 5^m + k,
   where 5^m ≤ n < 5^(m+1) and 0 ≤ k < 4·5^m.
   If k = 0, a(n) = n.
   If k > 0, a(n) is the smallest q·a(k) with prime q ≠ 5
   that is not already a term.  The sequence is indexed starting at 1. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/cache_has.h"

/* --------------------------------------------------------------------- */
/* Helper: ensure that at least min_limit primes are available             */
/* --------------------------------------------------------------------- */
static void ensure_prime_list(size_t min_limit,
                              uint64_t **primes,
                              size_t *cnt,
                              size_t *current_limit)
{
    if (*current_limit >= min_limit)
        return;                    /* already sufficient */

    /* discard old list */
    if (*primes != NULL)
        free(*primes);

    /* build a new list */
    build_primes_upto((idx_t)min_limit, primes, cnt);
    *current_limit = min_limit;
}

/* --------------------------------------------------------------------- */
/* Helper: test if val already occurs in cache[1..upto]                    */

/* --------------------------------------------------------------------- */
/* Main sequence function                                                */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;       /* undefined for n = 0 */

    /* static state: cache of computed values and prime list */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;          /* largest index computed so far */

    static uint64_t *prime_list = NULL;
    static size_t prime_cnt  = 0;
    static size_t prime_limit = 0;         /* numeric limit of primes built */

    /* grow cache if necessary */
    if (cache_len < (size_t)n) {
        size_t new_cap = n + 1;            /* index 0 unused, indices [1..n] valid */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {                  /* allocation failed – leave cache empty */
            free(cache);
            cache = NULL;
            return (seq_t)0;
        }
        cache = new_cache;

        /* compute missing terms from cache_len+1 up to n */
        for (idx_t i = (idx_t)(cache_len + 1); i <= n; ++i) {
            /* largest power of 5 not exceeding i */
            idx_t power = 1;
            while (power * (idx_t)5 <= i)
                power *= 5;
            idx_t k = i - power;

            seq_t val;
            if (k == 0) {
                val = (seq_t)i;             /* i itself is a power of 5 */
            } else {
                seq_t a_k = cache[(size_t)k];

                /* ensure at least some primes are available */
                if (prime_cnt == 0)
                    ensure_prime_list(100, &prime_list, &prime_cnt, &prime_limit);

                bool found = false;
                while (!found) {
                    for (size_t pi = 0; pi < prime_cnt; ++pi) {
                        uint64_t q = prime_list[pi];
                        if (q == 5)            /* prime must differ from 5 */
                            continue;

                        seq_t cand = a_k * (seq_t)q;
                        if (!cache_has(cache, (size_t)i - 1, cand)) {
                            val = cand;
                            found = true;
                            break;
                        }
                    }
                    if (!found) {             /* need more primes */
                        size_t new_limit = prime_limit * 2 + 1000;
                        ensure_prime_list(new_limit, &prime_list, &prime_cnt,
                                          &prime_limit);
                    }
                }
            }
            cache[(size_t)i] = val;
        }
        cache_len = (size_t)n;
    }
    return cache[(size_t)n];
}
