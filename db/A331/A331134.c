/*
 * A331134  a(n) = Sum_{primes p <= n} r_2(p)/4, where r_2(n) = A004018(n).
 * https://oeis.org/A331134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{primes p <= n} r₂(p)/4 * where r₂(p) = 4 if p==2; 8
 *            if p≡1 (mod 4); 0 otherwise. * So a(n) = (1 if 2≤n else 0) +
 *            2·[# of primes ≤ n that are ≡1 (mod 4)]. * * This function
 *            caches results up to the largest n requested so that * repeated
 *            calls (in any order) are fast. The cache is recom...
 * Range:     u8 n <= 1696, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* idx_t (unsigned 64‑bit) and seq_t (u8/16/32/64/128 or arbitrary‑precision) are
 * defined elsewhere in the build and are used here directly. */

seq_t a(idx_t n)
{
    /* Ensure we have at least one entry.  a(0) is unused but convenient. */
    static seq_t *cache = NULL;         /* cumulative a(i) values */
    static idx_t   cache_limit = 0;     /* largest index for which cache is valid */

    if (n == 0)            /* offset starts at 1, but return 0 for n==0 safely */
        return 0;

    /* Extend cache if necessary.  We rebuild the entire array for simplicity
     * because n is small (≤ 10000 during tests). */
    if (n > cache_limit) {
        /* Allocate new arrays */
        size_t sz = (size_t)(n + 1);
        seq_t   *new_cache = (seq_t*)malloc(sz * sizeof(seq_t));
        bool    *is_prime  = (bool  *)malloc(sz * sizeof(bool));

        if (!new_cache || !is_prime) {             /* allocation failure */
            free(new_cache);
            free(is_prime);
            return 0;                              /* graceful fallback */
        }

        /* Initialise primality array */
        is_prime[0] = false;
        if (sz > 1) is_prime[1] = false;
        for (idx_t i = 2; i < n + 1; ++i)
            is_prime[(size_t)i] = true;

        /* Classic Ératosthenes sieve */
        for (idx_t i = 2; i * i <= n; ++i) {
            if (is_prime[(size_t)i]) {
                for (idx_t j = i * i; j <= n; j += i)
                    is_prime[(size_t)j] = false;
            }
        }

        /* Build cumulative a(·) array */
        new_cache[0] = 0;
        for (idx_t i = 1; i <= n; ++i) {
            new_cache[(size_t)i] = new_cache[(size_t)(i-1)];   /* inherit previous */
            if (i >= 2 && is_prime[(size_t)i]) {
                if (i == 2)                           /* r₂(2)/4 = 1 */
                    new_cache[(size_t)i] += 1;
                else if ((i & 3U) == 1)               /* p≡1 (mod 4), r₂=8 */
                    new_cache[(size_t)i] += 2;
                /* p≡3 (mod 4) contributes 0 */
            }
        }

        /* Replace old cache */
        free(cache);
        cache = new_cache;
        cache_limit = n;
    }

    return cache[(size_t)n];
}
