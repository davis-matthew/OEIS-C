/*
 * A393966  Numbers k which are not divisible by their digital root
 *          A010888(k).
 * https://oeis.org/A393966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth positive integer k such that k % digital_root(k)
 *            != 0
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* static cache for already computed terms */
static seq_t *cache = NULL;      /* array of terms */
static size_t cache_cap = 0;     /* allocated capacity */
static size_t cache_cnt = 0;     /* number of stored terms  */
static idx_t next_start = 1;     /* next integer to test   */

/* maximum value storable in seq_t */
static const idx_t max_seq = (idx_t)((~(seq_t)0));

static void grow_cache(size_t need)
{
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
}

static void cache_append(seq_t val)
{
    grow_cache(cache_cnt + 1);
    cache[cache_cnt++] = val;
}

/* digital root for a positive integer k */
static seq_t digital_root(idx_t k)
{
    return (seq_t)(1 + ((k - 1) % 9));
}

/* Main function required by the build framework. */
seq_t a(idx_t n)
{
    while (cache_cnt < (size_t)n) {
        idx_t k = next_start;
        seq_t dr = digital_root(k);
        if (k % (idx_t)dr != 0) {
            if (k <= max_seq)
                cache_append((seq_t)k);
        }
        ++next_start;
    }
    return cache[(size_t)n - 1];
}
