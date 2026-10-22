/*
 * A373083  a(1) = 1; for n > 1, a(n) = a(n-1) + n if n is prime, else a(n) =
 *          a(n-1) + largest divisor of n < n.
 * https://oeis.org/A373083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + (n if n is prime else largest proper divisor of
 *            n). Initial value: a(1) = 1
 * Range:     u8 n <= 28, u16 n <= 511, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

#ifdef __cplusplus

#else

#endif

/* Global static state */
static seq_t*  cache      = NULL;   /* a(0) .. a(max_cached) */
static idx_t   cache_size = 0;     /* allocated size (>= cache_len+1) */
static idx_t   cached_max = 0;     /* highest n for which cache[n] is valid */

static idx_t* spf = NULL;          /* smallest prime factor array, 0..max_spf */

/* Recreate the spf table up to 'limit' (inclusive).  */
static void recompute_spf(idx_t limit)
{
    if (spf != NULL) { free(spf); }
    spf = (idx_t*)calloc((size_t)limit + 1, sizeof(idx_t));
    if (!spf) exit(1); /* OOM */

    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;                     /* prime itself */
            if (i <= limit / i) {
                for (idx_t j = i * i; j <= limit; j += i) {
                    if (spf[j] == 0) spf[j] = i;
                }
            }
        }
    }
}

/* Ensure the cache can hold an entry for n, recomputing data if needed. */
static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;          /* already sufficient */

    /* Resize cache array */
    size_t new_size = (size_t)n + 1;      /* indices 0..n */
    seq_t* new_cache = alloc_seq_array(new_size);
    if (!new_cache) exit(1);              /* OOM */

    /* Copy existing data */
    for (idx_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];

    /* Free old cache if any */
    if (cache) free_seq_array(cache);
    cache      = new_cache;
    cache_size = new_size;

    /* Recompute spf up to n if necessary */
    recompute_spf(n);

    if (cached_max == 0) {           /* first time */
        cache[0] = 0;                 /* unused but safe */
        if (n >= 1) cache[1] = 1;
        cached_max = (n >= 1) ? 1 : 0;
    }

    /* Fill in new entries up to n */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        seq_t add;
        if (spf[i] == i) {                /* prime */
            add = (seq_t)i;
        } else {                          /* composite: largest proper divisor = i / spf[i] */
            add = (seq_t)(i / spf[i]);
        }
        cache[i] = cache[i - 1] + add;
    }
    cached_max = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for n==0, but safe */
    ensure_cache(n);
    return cache[n];
}
