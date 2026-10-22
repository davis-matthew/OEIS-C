/*
 * A321001  Primes which contain the fax number of the beast (667).
 * https://oeis.org/A321001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 14, u32 n <= 24, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   contains667, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes containing the decimal digit string 667.
   The function searches incrementally for the next qualifying prime
   and caches the results for future calls. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/contains667.h"

#ifdef __cplusplus
/* In C++ a cast from void* to seq_t* is required. */
#define ALLOC(sz) reinterpret_cast<seq_t*>(malloc(sz))
#define REALLOC(ptr, sz) reinterpret_cast<seq_t*>(realloc((void*)(ptr), sz))
#else
/* In C the void* can be assigned to any pointer type directly. */
#define ALLOC(sz) malloc(sz)
#define REALLOC(ptr, sz) realloc((void*)(ptr), sz)
#endif

/* Cached sequence data. */
static seq_t *seq_cache   = NULL;
static size_t  cache_cap  = 0;   /* allocated slots */
static idx_t   cache_len  = 0;   /* stored terms */

/* Next number to test for primality. */
static seq_t next_candidate = (seq_t)1667;

/* Ensure that seq_cache contains at least n terms. */
static void ensure_terms(idx_t n)
{
    if (seq_cache == NULL) {
        cache_cap = 16;
        seq_cache = ALLOC(cache_cap * sizeof(seq_t));
    }

    while (cache_len < n) {
        seq_t candidate = next_candidate;
        while (true) {
            if (contains667(candidate) && is_prime_big(candidate)) {
                if (cache_len >= cache_cap) {
                    cache_cap *= 2;
                    seq_cache = REALLOC(seq_cache, cache_cap * sizeof(seq_t));
                }
                seq_cache[cache_len++] = candidate;
                next_candidate = candidate + (seq_t)1;
                break;
            }
            candidate++;                /* try next integer */
        }
    }
}

/* External entry point. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                 /* undefined for n < 1 */

    ensure_terms(n);
    return seq_cache[n-1];
}
