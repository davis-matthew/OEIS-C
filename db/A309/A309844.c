/*
 * A309844  Primes of the form n^4 + n^2 + 3.
 * https://oeis.org/A309844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 5, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th prime of the form m^4 + m^2 + 3 (OEIS A309844). */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"

static seq_t *prime_cache = NULL;   /* cached primes */
static size_t   cache_cap   = 0;    /* capacity of prime_cache */
static size_t   cache_len   = 0;    /* number of cached primes */
static idx_t    next_m      = 0;    /* next integer m to test */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* undefined for offset 0, but harmless */

    while (cache_len < (size_t)n) {
        seq_t m   = (seq_t)next_m;
        seq_t m2  = m * m;
        seq_t m4  = m2 * m2;
        seq_t val = m4 + m2 + (seq_t)3;

        if (is_prime_big(val)) {
            if (cache_len == cache_cap) {
                size_t old_cap = cache_cap;
                size_t new_cap = (cache_cap == 0 ? 8 : cache_cap * 2);
                prime_cache         = seq_realloc(prime_cache, old_cap, new_cap);
                cache_cap           = new_cap;
            }
            prime_cache[cache_len++] = val;
        }
        ++next_m;
    }
    return prime_cache[n - 1];
}
