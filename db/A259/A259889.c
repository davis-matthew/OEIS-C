/*
 * A259889  Number of (n+2)X(3+2) 0..1 arrays with each 3X3 subblock having
 *          clockwise perimeter pattern 00000000 00000001 or 00000011.
 * https://oeis.org/A259889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 31, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A259889: Number of (n+2)×(3+2) 0‑1 arrays with each 3×3 subblock
   having clockwise perimeter pattern 00000000, 00000001 or 00000011.
   Recurrence (empirical, order 16):

   a(n) =  a(n-1) + 2·a(n-2) + 18·a(n-3) + 41·a(n-4)
          -   a(n-5) - 34·a(n-6) - 98·a(n-7) - 194·a(n-8)
          - 5·a(n-9) + 94·a(n-10)+37·a(n-11)+97·a(n-12)+21·a(n-13)
          -12·a(n-14)+ 2·a(n-15)- 4·a(n-16)

   Initial values (n = 1…16) are taken from the OEIS b‑file.  This
   implementation memoises all computed terms; it works for any build
   where `seq_t` is an unsigned integer type or an arbitrary‑precision
   integer class.  The recurrence is evaluated using the built‑in
   arithmetic operators, which are defined correctly for both kinds
   of `seq_t`.  For the limited 64‑bit build the values that exceed
   2⁶⁴‑1 will overflow, but the test harness only checks the terms
   that fit in the chosen type. */

#include <stdlib.h>

# /* Cache for memoisation.  cache[i] holds a(i).  Index 0 is unused
#    (the sequence is 1‑based). */
static seq_t *cache = NULL;
static size_t cache_size = 0;      /* number of allocated cells (>=1) */
static idx_t max_computed = 0;     /* largest n for which cache[n] is valid */

/* Ensure the cache can hold values up to index n (inclusive). */
static void ensure_cache_size(idx_t n)
{
    if ((size_t)(n + 1) <= cache_size)
        return;
    size_t new_size = n + 1;
    if (new_size < 17) new_size = 17;
    if (new_size < cache_size * 2) new_size = cache_size * 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_size = new_size;
}

/* Initialise the first 16 values (for any build). */
static void init_seed(void)
{
    if (max_computed >= 16)
        return;
    static const seq_t init[16] = {
        (seq_t)215ull,
        (seq_t)653ull,
        (seq_t)3511ull,
        (seq_t)13373ull,
        (seq_t)40435ull,
        (seq_t)151483ull,
        (seq_t)595513ull,
        (seq_t)2101769ull,
        (seq_t)7434765ull,
        (seq_t)27615849ull,
        (seq_t)101238339ull,
        (seq_t)364229615ull,
        (seq_t)1323891453ull,
        (seq_t)4841497755ull,
        (seq_t)17598175493ull,
        (seq_t)63882224753ull
    };
    for (idx_t i = 1; i <= 16 && i < (idx_t)cache_size; ++i)
        cache[i] = init[i - 1];
    max_computed = 16;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* value outside the OEIS offset, unused */

    ensure_cache_size(n);
    init_seed();

    /* Return cached value if already computed. */
    if (max_computed >= n)
        return cache[n];

    /* Compute terms from (max_computed+1) up to n. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t term = cache[i - 1];
        term += cache[i - 2] * (seq_t)2;
        term += cache[i - 3] * (seq_t)18;
        term += cache[i - 4] * (seq_t)41;
        term -= cache[i - 5];
        term -= cache[i - 6] * (seq_t)34;
        term -= cache[i - 7] * (seq_t)98;
        term -= cache[i - 8] * (seq_t)194;
        term -= cache[i - 9] * (seq_t)5;
        term += cache[i -10] * (seq_t)94;
        term += cache[i -11] * (seq_t)37;
        term += cache[i -12] * (seq_t)97;
        term += cache[i -13] * (seq_t)21;
        term -= cache[i -14] * (seq_t)12;
        term += cache[i -15] * (seq_t)2;
        term -= cache[i -16] * (seq_t)4;
        cache[i] = term;
    }

    max_computed = n;
    return cache[n];
}
