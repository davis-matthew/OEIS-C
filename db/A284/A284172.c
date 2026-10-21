/*
 * A284172  a(1)=2; thereafter a(n+1) = a(n)+i if a(n) is a prime and
 *          a(1),...,a(n) contains i primes, or a(n+1) = a(n)-i if a(n) is
 *          composite and a(1),...,a(n) contains i composite terms.
 * https://oeis.org/A284172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 191, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=2; for n>1 let i be the count of prime terms among a(1…n‑1) if a(n‑1) is prime, otherwise the count of composite terms. Then  
   a(n)=a(n‑1)+i  if a(n‑1) is prime,  
   a(n)=a(n‑1)‑i  if a(n‑1) is composite. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Static cache for previously computed terms */
    static seq_t *cache = NULL;          /* 1‑based array, cache[1] = a(1) */
    static size_t cache_cap = 0;         /* allocated size (indices 1…cache_cap-1) */
    static idx_t max_computed = 0;       /* largest index that is currently set */
    static idx_t prime_cnt = 0;          /* primes seen up to max_computed */
    static idx_t comp_cnt  = 0;          /* composites seen up to max_computed */

    if (n < 1) return 0;                 /* undefined for n<1 */

    /* First call: set up cache for at least n */
    if (cache == NULL) {
        cache_cap = n + 1;
        if (cache_cap < 2) cache_cap = 2;                /* at least room for a(1) */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;                        /* allocation failure unlikely */
        cache[1] = (seq_t)2;                         /* a(1) */
        prime_cnt = 1;
        comp_cnt  = 0;
        max_computed = 1;
    }

    /* Grow cache if it does not cover n */
    if (n >= cache_cap) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                          /* allocation failure unlikely */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate missing terms up to n */
    for (idx_t idx = max_computed + 1; idx <= n; ++idx) {
        seq_t prev = cache[idx - 1];
        bool prev_is_prime = is_prime_big(prev);

        idx_t step = prev_is_prime ? prime_cnt : comp_cnt;
        seq_t next = prev_is_prime ? (prev + (seq_t)step)
                                   : (prev - (seq_t)step);
        cache[idx] = next;

        /* Update counts for the new term */
        bool next_is_prime = is_prime_big(next);
        if (next_is_prime) {
            ++prime_cnt;
        } else if (next > (seq_t)1) {  /* treat 1 as neither prime nor composite */
            ++comp_cnt;
        }
    }
    max_computed = n;

    return cache[n];
}
