/*
 * A387151  a(n) = n*n! / Product_{k=1..n} radical(k), where radical(n) is the
 *          product of distinct prime factors of n, cf. A007947.
 * https://oeis.org/A387151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*n! / ∏_{k=1..n}radical(k)
 * Range:     u8 n <= 10, u16 n <= 23, u32 n <= 39, u64 n <= 74, u128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;

    /* static state for memoisation */
    static idx_t max_n = 0;
    static uint64_t *rad = NULL;      /* radical values */
    static seq_t *cache = NULL;      /* sequence values */

    if (n > max_n) {
        /* allocate new buffers */
        size_t cap = (size_t)n + 1;
        uint64_t *new_rad   = (uint64_t*)malloc(cap * sizeof(uint64_t));
        seq_t    *new_cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!new_rad || !new_cache) return (seq_t)0;  /* allocation failure */

        /* initialise radical array */
        for (size_t i = 0; i <= (size_t)n; ++i) new_rad[i] = 1;

        /* simple sieve to compute radicals */
        bool *is_prime = (bool*)malloc(cap * sizeof(bool));
        for (size_t i = 0; i <= (size_t)n; ++i) is_prime[i] = true;
        is_prime[0] = is_prime[1] = false;

        for (uint32_t p = 2; p <= n; ++p) {
            if (is_prime[p]) {
                for (size_t j = p; j <= (size_t)n; j += p) {
                    if (j != (size_t)p) is_prime[j] = false;
                    new_rad[j] *= p;
                }
            }
        }
        free(is_prime);

        /* compute sequence values */
        new_cache[0] = (seq_t)0;
        seq_t prod = 1;
        for (idx_t i = 1; i <= n; ++i) {
            seq_t mul = (seq_t)i / (seq_t)new_rad[i];   /* n/rad(n) is integer */
            prod *= mul;
            new_cache[i] = (seq_t)i * prod;              /* a(n) = n * prod */
        }

        /* replace old buffers */
        free(rad);
        free(cache);
        rad = new_rad;
        cache = new_cache;
        max_n = n;
    }

    return cache[(size_t)n];
}
