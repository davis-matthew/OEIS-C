/*
 * A259968  a(n) = a(n-1) + a(n-2) + a(n-4), with a(1)=1, a(2)=1, a(3)=3,
 *          a(4)=6.
 * https://oeis.org/A259968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 40, u64 n <= 79, u128 n <= 158,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A259968
 * a(n) = a(n-1) + a(n-2) + a(n-4)
 * a(1)=1, a(2)=1, a(3)=3, a(4)=6
 * The following code memoises the values in an array; it works for all
 * supported seq_t (unsigned integer or arbitrary‑precision class) and
 * for all idx_t (uint64_t).  The routine is O(n) in the largest n
 * requested and uses only the operations allowed by the build.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

static seq_t *cache = NULL;   /* values a(1)... */
static size_t cache_cap = 0;  /* allocated capacity      */
static size_t cache_size = 0; /* number of stored items   */

/* Grow the cache to hold at least 'needed' elements. */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;

    cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (!cache)
        abort();                    /* allocation failed */
    cache_cap = needed;
}

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1, but treat safely. */
    if (n == 0)
        return (seq_t)0;

    /* Initialise the first four values once. */
    if (cache_size < 4) {
        ensure_capacity(4);
        cache[0] = (seq_t)1;  /* a(1) */
        cache[1] = (seq_t)1;  /* a(2) */
        cache[2] = (seq_t)3;  /* a(3) */
        cache[3] = (seq_t)6;  /* a(4) */
        cache_size = 4;
    }

    /* Return pre‑computed value if available. */
    if ((size_t)n <= cache_size)
        return cache[n - 1];

    /* Grow cache up to the required index. */
    ensure_capacity((size_t)n);

    /* Compute terms from current size+1 up to n. */
    for (size_t i = cache_size + 1; i <= (size_t)n; ++i) {
        /* i is a 1‑based index: a(i) = a(i-1)+a(i-2)+a(i-4) */
        /* a(i-1) → cache[i-2], a(i-2) → cache[i-3], a(i-4) → cache[i-5] */
        seq_t term = cache[i - 2] + cache[i - 3] + cache[i - 5];
        cache[i - 1] = term;
        ++cache_size;
    }

    return cache[n - 1];
}
