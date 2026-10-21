/*
 * A299483  Irregular triangle read by rows in which row n lists the odd
 *          divisors of n in increasing order together with the even divisors
 *          of n in decreasing order.
 * https://oeis.org/A299483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) enumerates the irregular triangle: * row m lists odd
 *            divisors of m (increasing) followed by even divisors of m
 *            (decreasing). * The sequence is the concatenation of all rows in
 *            order
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static idx_t next_m = 1;

static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    if (new_cap < needed) new_cap = needed;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

static void append_value(seq_t val)
{
    if (cache_len >= cache_cap) grow_cache(cache_len + 1);
    cache[cache_len++] = val;
}

static void produce_row(idx_t m)
{
    /* collect divisors */
    seq_t odd[2048];
    size_t odd_len = 0;
    seq_t even[2048];
    size_t even_len = 0;
    for (idx_t d = 1; d <= m / d; ++d) {
        if (m % d == 0) {
            idx_t e = m / d;
            if (d & 1) {
                odd[odd_len++] = (seq_t)d;
            } else {
                even[even_len++] = (seq_t)d;
            }
            if (e != d) {
                if (e & 1) {
                    odd[odd_len++] = (seq_t)e;
                } else {
                    even[even_len++] = (seq_t)e;
                }
            }
        }
    }
    /* sort odd ascending */
    for (size_t i = 1; i < odd_len; ++i) {
        seq_t key = odd[i];
        size_t j = i;
        while (j > 0 && odd[j - 1] > key) {
            odd[j] = odd[j - 1];
            --j;
        }
        odd[j] = key;
    }
    /* sort even ascending */
    for (size_t i = 1; i < even_len; ++i) {
        seq_t key = even[i];
        size_t j = i;
        while (j > 0 && even[j - 1] > key) {
            even[j] = even[j - 1];
            --j;
        }
        even[j] = key;
    }
    /* append odd ascending */
    for (size_t i = 0; i < odd_len; ++i) {
        append_value(odd[i]);
    }
    /* append even descending */
    for (size_t i = even_len; i-- > 0; ) {
        append_value(even[i]);
    }
}

static void precompute_upto(idx_t target_n)
{
    while (cache_len < target_n) {
        produce_row(next_m);
        ++next_m;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety, offset is 1 */
    precompute_upto(n);
    return cache[n - 1];
}
