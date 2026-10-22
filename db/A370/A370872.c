/*
 * A370872  Positive integers m such that c(0) > c(1) >= c(2), where c(k) =
 *          number of k's in the ternary representation of m.
 * https://oeis.org/A370872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A370872: positive integers m such that c(0) > c(1) >= c(2)
   where c(k) is the number of digits equal to k in the ternary expansion of m.   */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* cached list of numbers that satisfy the condition */
static seq_t *cache = NULL;
static size_t cache_len = 0;     /* number of valid elements in cache   */
static size_t cache_cap = 0;     /* current capacity of the cache      */
static idx_t last_checked_m = 0; /* largest m that was examined so far */

/* enlarge the cache so that it can hold at least 'need' elements */
static void ensure_capacity(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < need) new_cap = need;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) exit(1); /* out of memory */
    if (cache) {
        memcpy(new_cache, cache, cache_len * sizeof(seq_t));
        free(cache);
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* test whether m satisfies the ternary digit condition
   c(0) > c(1) >= c(2)                                             */
static bool good(seq_t m)
{
    unsigned c0 = 0, c1 = 0, c2 = 0;
    seq_t t = m;
    while (t > 0) {
        unsigned d = (unsigned)(t % 3);
        if (d == 0) ++c0;
        else if (d == 1) ++c1;
        else ++c2;
        t /= 3;
    }
    return (c0 > c1) && (c1 >= c2);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* offset starts at 1, n=0 is unused */

    /* return cached value if already computed */
    if (cache_len >= (size_t)n) return cache[n - 1];

    /* generate subsequent good numbers until we have at least n of them */
    idx_t m = last_checked_m + 1;
    while (cache_len < (size_t)n) {
        if (good(m)) {
            ensure_capacity(cache_len + 1);
            cache[cache_len++] = (seq_t)m;
        }
        ++m;
    }

    /* the last checked m was the one that made the loop stop,
       which is the previous value of m */
    last_checked_m = m - 1;

    return cache[n - 1];
}
