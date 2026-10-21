/*
 * A259519  Number of (n+1)X(3+1) 0..1 arrays with each 2X2 subblock having
 *          clockwise pattern 0000 0001 0011 0101 or 1111.
 * https://oeis.org/A259519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14*a(n-1) - 45*a(n-2) - 18*a(n-3) + 249*a(n-4) -
 *            239*a(n-5) - 128*a(n-6) + 184*a(n-7) + 10*a(n-8) - 20*a(n-9) -
 *            2*a(n-10) (n > 10) Initial values: a(1)=120, a(2)=1122,
 *            a(3)=10148, a(4)=93028, a(5)=851604, a(6)=7802386,
 *            a(7)=71482990, a(8)=654942820, a(9)=6000746068,
 *            a(10)=54980553426 The r...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* The external definitions of seq_t and idx_t are supplied by the build. */

static seq_t *cache = NULL;       /* memoisation array indexed from 1 */
static size_t   cache_cap = 0;     /* current allocated size */
static idx_t    last_computed = 0; /* highest index that has been computed */

/* Helper to grow the cache to at least @new_cap elements (index 0 is unused). */
static void ensure_cache_capacity(size_t new_cap)
{
    if (new_cap <= cache_cap) return;
    /* Allocate a new array and copy existing values */
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* In a real program we would handle allocation failure. */
        abort();
    }
    cache = new_cache;
    /* Initialise any new indices to 0 */
    for (size_t i = cache_cap; i < new_cap; ++i) cache[i] = 0;
    cache_cap = new_cap;
}

/* Main function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1, but handle n==0 safely */

    /* Ensure we have the initial 10 terms stored. */
    if (cache_cap == 0) {
        ensure_cache_capacity(11); /* indices 0..10 */
        cache[1]  = (seq_t)120;
        cache[2]  = (seq_t)1122;
        cache[3]  = (seq_t)10148;
        cache[4]  = (seq_t)93028;
        cache[5]  = (seq_t)851604;
        cache[6]  = (seq_t)7802386;
        cache[7]  = (seq_t)71482990;
        cache[8]  = (seq_t)654942820;
        cache[9]  = (seq_t)6000746068;
        cache[10] = (seq_t)54980553426;
        last_computed = 10;
    }

    /* If the requested term is already computed, return it immediately. */
    if (n <= last_computed) {
        return cache[n];
    }

    /* Grow cache to accommodate the requested index. */
    ensure_cache_capacity((size_t)n + 1);

    /* Compute terms successively up to n, using the recurrence. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* Evaluate the recurrence in the order shown in the formula
           to keep intermediate sums non‑negative (unsigned arithmetic). */
        seq_t term = (seq_t)14 * cache[i - 1];
        term -= (seq_t)45 * cache[i - 2];
        term -= (seq_t)18 * cache[i - 3];
        term += (seq_t)249 * cache[i - 4];
        term -= (seq_t)239 * cache[i - 5];
        term -= (seq_t)128 * cache[i - 6];
        term += (seq_t)184 * cache[i - 7];
        term += (seq_t)10  * cache[i - 8];
        term -= (seq_t)20  * cache[i - 9];
        term -= (seq_t)2   * cache[i -10];
        cache[i] = term;
    }

    last_computed = n;
    return cache[n];
}
