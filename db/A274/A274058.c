/*
 * A274058  Relative of Hofstadter Q-sequence: a(n) = max(0, n+32478) for n <=
 *          0; a(n) = a(n-a(n-1)) + a(n-a(n-2)) + a(n-a(n-3)) for n > 0.
 * https://oeis.org/A274058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 61, u32 n <= 7944, u64 n <= 7944,
 *            u128 n <= 7944, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A274058
 * a(n) = max(0, n + 32478)          for n <= 0
 * a(n) = a(n – a(n-1)) + a(n – a(n-2))
 *        + a(n – a(n-3))           for n > 0
 * Offset: 1 (a(1) is the first term listed)
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static cache; cache[i] holds a(i) for 0 <= i <= cache_max */
static seq_t *cache = NULL;          /* array of a(i) */
static idx_t  cache_size = 0;        /* allocated array size (>= cache_max+1) */
static idx_t  cache_max  = 0;        /* greatest n for which a(n) is computed */

/* Ensure cache has room for indices up to n (inclusive) */
static void
ensure_cache_size(idx_t n)
{
    if (cache_size > n) return;       /* already enough space */

    /* Grow to next power of two greater than n */
    idx_t new_size = cache_size ? cache_size : 1;
    while (new_size <= n) new_size <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, (new_size + 1) * sizeof(seq_t));
    if (!tmp) exit(1);                 /* out of memory – abort */
    cache = tmp;
    cache_size = new_size;
    /* No need to initialize new elements – they will be set during computation */
}

/* Return a(k) for a signed index k (k can be negative).  */
static inline seq_t
a_signed(int64_t k)
{
    if (k >= 0) {           /* non‑negative index – cached value is available */
        return cache[(idx_t)k];
    } else {                /* use the definition for n <= 0 */
        int64_t v = k + 32478;
        return (seq_t)(v > 0 ? v : 0);
    }
}

/* Public API: compute a(n) for n >= 1 (a(0) is used internally). */
seq_t a(idx_t n)
{
    if (n == 0) return 32478;          /* explicit base value for n = 0 */

    ensure_cache_size(n);

    /* If we already computed up to n, just return it. */
    if (n <= cache_max) return cache[n];

    /* Make sure a(0) is defined for the recurrence.  */
    if (cache[0] == 0) cache[0] = 32478;

    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t a_prev1 = a_signed((int64_t)i - 1);
        seq_t a_prev2 = a_signed((int64_t)i - 2);
        seq_t a_prev3 = a_signed((int64_t)i - 3);

        int64_t idx1 = (int64_t)i - (int64_t)a_prev1;
        int64_t idx2 = (int64_t)i - (int64_t)a_prev2;
        int64_t idx3 = (int64_t)i - (int64_t)a_prev3;

        seq_t t1 = a_signed(idx1);
        seq_t t2 = a_signed(idx2);
        seq_t t3 = a_signed(idx3);

        cache[i] = t1 + t2 + t3;
    }

    cache_max = n;
    return cache[n];
}
