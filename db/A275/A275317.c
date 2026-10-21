/*
 * A275317  Prime numbers of the form 100*n+57.
 * https://oeis.org/A275317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime congruent to 57 modulo 100
 * Range:     u8 n <= 1, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   is_prime_big, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Cache of already found primes of the form 100*k+57 */
static seq_t *prime_cache = NULL;
static size_t cache_len   = 0;
static size_t cache_cap   = 0;
static idx_t  next_k      = 1;      /* next k to examine (p = 100*k+57) */

/* Return the nth term of A275317 (1‑based indexing). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined, but harmless */

    /* Generate primes until we have at least n cached */
    while (cache_len < (size_t)n) {
        seq_t candidate = (seq_t)(100 * (seq_t)next_k + 57);
        if (is_prime_big(candidate)) {   /* primality test using built‑in routine */
            /* Grow cache if necessary */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
                seq_t *tmp = seq_realloc(prime_cache, cache_cap, new_cap);
                prime_cache = tmp;
                cache_cap   = new_cap;
            }
            prime_cache[cache_len++] = candidate;
        }
        next_k++;
    }

    return prime_cache[(size_t)n - 1];
}
