/*
 * A396856  Union of primes and perfect powers.
 * https://oeis.org/A396856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th element of the union of the prime numbers and
 *            the perfect powers (including 1)
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    /* 0‑based cache size */
    static seq_t *cache = NULL;          /* array of computed values   */
    static idx_t cache_len = 0;          /* number of values in cache  */
    static idx_t limit   = 0;            /* upper bound used last time */

    if (n <= 0) return 0;                /* undefined for n <= 0 */

    /* Return cached value if already computed */
    if (n <= cache_len) return cache[n - 1];

    /* ----------------------------------------------------------
       Extend computation until at least n elements are known.
       ---------------------------------------------------------- */
    idx_t target = n;
    /* Start with a rough estimate, at least n*10, or larger than previously used */
    if (limit < target * 10) limit = target * 10;

    /* Temporary arrays for prime and perfect‑power flags */
    char *is_prime   = NULL;
    char *is_power   = NULL;
    idx_t  total_cnt = 0;       /* number of union elements up to 'limit' */

    while (1) {
        /* Allocate flag arrays */
        is_prime = (char*)malloc((limit + 1) * sizeof(char));
        is_power = (char*)malloc((limit + 1) * sizeof(char));
        if (!is_prime || !is_power) {
            /* allocation failure – return 0 (should not happen in test) */
            free(is_prime); free(is_power);
            return 0;
        }

        /* --- 1. Sieve of Eratosthenes --------------------------------- */
        memset(is_prime, 1, (limit + 1) * sizeof(char));
        is_prime[0] = is_prime[1] = 0;
        for (idx_t p = 2; p <= limit / p; ++p) {
            if (is_prime[p]) {
                for (idx_t j = p * p; j <= limit; j += p) {
                    is_prime[j] = 0;
                }
            }
        }

        /* --- 2. Perfect powers ---------------------------------------- */
        memset(is_power, 0, (limit + 1) * sizeof(char));
        for (idx_t base = 2; base <= limit / base; ++base) {           /* base >= 2 */
            idx_t val = base * base;                                 /* exponent 2 */
            while (val <= limit) {
                is_power[val] = 1;
                if (val > limit / base) break;                       /* overflow guard */
                val *= base;
            }
        }

        /* --- 3. Count union elements --------------------------------- */
        total_cnt = 1; /* counting 1 (perfect power 1^b) */
        for (idx_t i = 2; i <= limit; ++i) {
            if (is_prime[i] || is_power[i]) ++total_cnt;
        }

        /* If we have enough elements, break; otherwise enlarge limit */
        if (total_cnt >= target) break;

        /* Not enough – free and double the limit */
        free(is_prime); free(is_power);
        limit = limit * 2;
        if (limit < target * 10) limit = target * 10;   /* keep lower bound */
    }

    /* ----------------------------------------------------------
       Build the cache array with the discovered elements.
       ---------------------------------------------------------- */
    seq_t *new_cache = (seq_t*)malloc(total_cnt * sizeof(seq_t));
    if (!new_cache) {
        free(is_prime); free(is_power);
        return 0; /* allocation failure (unlikely) */
    }

    idx_t pos = 0;
    new_cache[pos++] = (seq_t)1;          /* include 1 first */
    for (idx_t i = 2; i <= limit; ++i) {
        if (is_prime[i] || is_power[i]) {
            new_cache[pos++] = (seq_t)i;
        }
    }

    /* Sanity check */
    if (pos != total_cnt) {
        free(is_prime); free(is_power);
        free(new_cache);
        return 0;
    }

    /* Replace old cache */
    free(cache);
    cache = new_cache;
    cache_len = total_cnt;
    /* limit remains valid for future extensions */

    /* Free temporary arrays */
    free(is_prime);
    free(is_power);

    return cache[n - 1];
}
