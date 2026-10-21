/*
 * A251745  8-step Fibonacci sequence starting with 0,1,0,0,0,0,0,0.
 * https://oeis.org/A251745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+8) = a(n)+a(n+1)+a(n+2)+a(n+3)+a(n+4)+a(n+5)+a(n+6)+a(n+7);
 *            Initial values: a(0)=0, a(1)=1, a(2)=0, a(3)=0, a(4)=0, a(5)=0,
 *            a(6)=0, a(7)=0
 * Range:     u8 n <= 16, u16 n <= 24, u32 n <= 40, u64 n <= 72, u128 n <= 136,
 *            big any n
 * Helpers:   grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                     /* for size_t */
#include "../utils/grow_seq_array.h"    /* provides grow_seq_array() */

static seq_t *cache = NULL;            /* dynamic array holding computed terms */
static size_t cap = 0;                 /* allocated capacity (in elements) */
static size_t len = 0;                 /* number of terms that have actually been computed (len >= 8 after first call) */

/* Ensure that the cache can store at least (n+1) terms and initialise
   the base values if it has never been initialised before. */
static void
ensure_cache(idx_t n)
{
    /* Allocate or grow the backing array if necessary. */
    if (cap <= (size_t)n) {
        grow_seq_array(&cache, &cap, (size_t)n + 1);
    }

    /* Initialize the first eight terms on first use. */
    if (len == 0) {
        cache[0] = 0;
        cache[1] = 1;
        for (idx_t i = 2; i < 8; ++i) {
            cache[i] = 0;
        }
        len = 8;                      /* we have valid entries up to index 7 */
    }
}

/* Return the n‑th term of sequence A251745. */
seq_t
a(idx_t n)
{
    /* Prepare the cache for indices up to n. */
    ensure_cache(n);

    /* If we have already computed up to n, just return the cached value. */
    if ((size_t)n < len) {
        return cache[n];
    }

    /* Compute all missing entries up to n. */
    for (idx_t i = (idx_t)len; i <= n; ++i) {
        seq_t sum = 0;
        for (int k = 1; k <= 8; ++k) {
            sum += cache[i - k];
        }
        cache[i] = sum;
    }
    len = (size_t)n + 1;              /* update the extent of computed terms */

    return cache[n];
}
