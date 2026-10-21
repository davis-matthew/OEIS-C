/*
 * A258663  Numbers n such that 9n-1 is prime.
 * https://oeis.org/A258663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th integer m such that 9m - 1 is prime. *
 *            Computation: trial‑division primality testing of the value 9m-1.
 *            * Results are cached; any ordering of calls is supported
 * Range:     u8 n <= 10, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;      /* cached a(k) values, 1‑based indexing */
static size_t cache_len = 0;     /* number of cached values (cache_len >= a(n) for computed n) */
static size_t cache_cap = 0;     /* capacity of the cache array */
static seq_t next_candidate = 2; /* next m to test, starts at 2 (first term) */

/* Expand cache capacity to at least new_cap */
static void ensure_cache_cap(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    size_t new_cap_extended = new_cap * 2 + 10;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap_extended * sizeof(seq_t));
    if (!new_ptr) {
        /* allocation failure: terminate */
        abort();
    }
    cache = new_ptr;
    cache_cap = new_cap_extended;
}

/* Generate terms until cache_len >= n */
static void generate_terms(idx_t n)
{
    while (cache_len < (size_t)n) {
        seq_t m = next_candidate++;
        seq_t candidate = (seq_t)9 * m - (seq_t)1;
        if (is_prime_big(candidate)) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = m;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;          /* undefined, return 0 */
    if (cache == NULL) {
        /* lazy initialization */
        cache_cap = 20;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
    }
    generate_terms(n);
    return cache[n - 1];
}
