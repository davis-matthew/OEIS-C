/*
 * A296271  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-2)*b(n), where a(0) = 3, a(1) = 4, b(0) = 1, b(1) = 2, b(2) =
 *          5, and (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A296271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 39, u64 n <= 85, u128 n <= 177,
 *            big any n
 * Helpers:   binary_search_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1)+a(n-2)+b(n-2)*b(n), a(0)=3, a(1)=4,
   b(0)=1, b(1)=2, b(2)=5; a and b are increasing complementary sequences. */

#include <stdlib.h>
#include "../utils/binary_search_seq_t.h"

/* static storage for memoised sequence values */
static seq_t *a_cache = NULL;
static seq_t *b_cache = NULL;
static size_t   cache_cap = 0;   /* maximum allocated entries   */
static size_t   cache_size = 0;  /* number of stored terms      */
static int      initialized = 0;

/* ensure capacity for at least (n+1) entries */
static void ensure_capacity(idx_t n)
{
    if (cache_cap > (size_t)n)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    if (new_cap <= (size_t)n + 1)
        new_cap = (size_t)n + 1;
    a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    b_cache = (seq_t *)realloc(b_cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* initialise the memoisation tables with the first three terms */
static void init(void)
{
    if (initialized)
        return;
    cache_cap = 8;
    a_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    b_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));

    a_cache[0] = (seq_t)3;
    a_cache[1] = (seq_t)4;
    a_cache[2] = (seq_t)12;        /* 3 + 4 + 1*5 */

    b_cache[0] = (seq_t)1;
    b_cache[1] = (seq_t)2;
    b_cache[2] = (seq_t)5;         /* complementary numbers */

    cache_size = 3;
    initialized = 1;
}

seq_t a(idx_t n)
{
    init();

    if ((idx_t)cache_size > n)   /* value already computed */
        return a_cache[n];

    ensure_capacity(n);

    for (idx_t k = (idx_t)cache_size; k <= n; ++k) {
        /* compute b(k) : smallest integer > b(k-1) not in a[0..k-1] */
        seq_t cand = b_cache[k - 1] + (seq_t)1;
        while (binary_search_seq_t(a_cache, (size_t)k, cand))
            cand = cand + (seq_t)1;
        b_cache[k] = cand;

        /* compute a(k) using the recurrence */
        seq_t a_k = a_cache[k - 1] + a_cache[k - 2] +
                    b_cache[k - 2] * b_cache[k];
        a_cache[k] = a_k;
    }

    return a_cache[n];
}
