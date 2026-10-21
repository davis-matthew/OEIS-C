/*
 * A252494  Numbers n such that all prime factors of n and n+1 are <= 11.
 *          (Related to the abc conjecture.)
 * https://oeis.org/A252494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n such that all prime factors of n and n+1 are <= 11 */
#include <stdlib.h>
#include <stdbool.h>

/* Return the n-th term (1‑based index) of OEIS A252494 */
seq_t a(idx_t n)
{
    /* Static cache of the sequence terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Helper to ensure cache capacity */
    if (cache == NULL) {
        cache_cap = 64;              /* initial capacity, more than enough */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    }
    /* Generate the sequence once */
    if (cache_len == 0) {
        const uint64_t primes[5] = {2, 3, 5, 7, 11};
        const uint64_t LIMIT   = 9800;      /* largest n in the sequence   */
        for (uint64_t m = 1; m <= LIMIT; ++m) {
            uint64_t x = m;
            for (int p = 0; p < 5; ++p) {
                while (x % primes[p] == 0) x /= primes[p];
            }
            if (x != 1) continue;          /* m has a prime factor > 11 */

            x = m + 1;
            for (int p = 0; p < 5; ++p) {
                while (x % primes[p] == 0) x /= primes[p];
            }
            if (x != 1) continue;          /* m+1 has a prime factor > 11 */

            /* Store m as term */
            if (cache_len == cache_cap) {
                cache_cap <<= 1;                  /* double capacity */
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)m;
        }
    }

    if (n == 0 || n > cache_len) {
        return (seq_t)0;   /* undefined beyond the known terms */
    }
    return cache[n - 1];
}
