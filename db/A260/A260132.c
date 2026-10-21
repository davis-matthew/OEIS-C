/*
 * A260132  Number of (n+2) X (2+2) 0..1 arrays with each 3 X 3 subblock
 *          having clockwise perimeter pattern 00010101 01010101 or 01010111.
 * https://oeis.org/A260132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the recurrence a(n) =
 *            a(n-1)+a(n-2)+5*a(n-3)+a(n-4)-3*a(n-5)-3*a(n-6) for n > 8, with
 *            initial values a(1)=58, a(2)=128, a(3)=342, a(4)=730, a(5)=1626,
 *            a(6)=3928, a(7)=8998, a(8)=20376
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 22, u64 n <= 49, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Links to the build‑supplied integer types. */
extern seq_t a(idx_t n);   /* forward declaration for the compiler */

/* Static cache for memoisation */
static seq_t *cache      = NULL;   /* array of computed terms, 1‑based index */
static idx_t cache_size  = 0;      /* number of elements currently allocated */
static idx_t max_computed = 0;     /* largest index that has been computed so far */

/* Ensure the cache can hold indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache_size > n)   /* already large enough */
        return;

    /* Allocate at least (n+1) elements (index 0 is unused) */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp)
        abort();          /* cannot allocate, terminate */
    cache = tmp;
    /* Initialise the newly allocated part (not needed for the recurrence itself,
       but keeps the cache containing 0 for unused positions). */
    for (idx_t i = cache_size; i < new_size; ++i)
        cache[i] = 0;
    cache_size = new_size;
}

/* Public routine requested by the problem statement. */
seq_t a(idx_t n)
{
    if (n < 1)            /* below the offset – undefined behaviour, return 0 */
        return 0;

    /* Predefined first 8 values (indices 1..8). The 0‑th entry is unused. */
    static const seq_t init[9] = {
        0ULL,            /* dummy 0‑th index */
        58ULL, 128ULL, 342ULL, 730ULL,
        1626ULL, 3928ULL, 8998ULL, 20376ULL
    };

    ensure_cache(n);

    /* Compute any missing values up to n, extending from the last computed
       index (max_computed) to n inclusive. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i <= 8) {
            cache[i] = init[i];
        } else {
            /* Use only seq_t arithmetic – the intermediate values fit in seq_t
               because the recurrence produces non‑negative results. */
            seq_t val = cache[i - 1] + cache[i - 2];
            val += (seq_t)5 * cache[i - 3];
            val += cache[i - 4];
            val -= (seq_t)3 * cache[i - 5];
            val -= (seq_t)3 * cache[i - 6];
            cache[i] = val;
        }
    }

    max_computed = n;
    return cache[n];
}
