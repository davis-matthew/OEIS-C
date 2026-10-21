/*
 * A247758  Numbers in decimal representation, such that in Latin their digits
 *          are in alphabetic order.
 * https://oeis.org/A247758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, word
 * Formula:   not stated
 * Range:     u8 n <= 94, u16 n <= 2083, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers in decimal representation whose digits are
   sorted in non‑decreasing order according to the Latin alphabet
   of the digit names: 2,9,8,4,5,6,7,3,1,0.
   This function enumerates such numbers in ascending decimal
   order and returns the nth term (1‑based). */

#include <stdlib.h>
#include <stdbool.h>

/* Rank of each digit in the custom alphabet order */
static const unsigned char rank[10] = {9, 8, 0, 7, 3, 4, 5, 6, 2, 1};

/* Check whether the decimal digits of x are in non‑decreasing
   order according to the custom alphabet. */
static bool digits_ok(uint64_t x)
{
    if (x == 0)
        return true;

    unsigned d[20];
    int len = 0;
    while (x) {
        d[len++] = x % 10;
        x /= 10;
    }
    /* d[0] is the least significant digit.  Check from most significant. */
    for (int i = len - 1; i > 0; --i) {
        if (rank[d[i]] > rank[d[i - 1]])
            return false;
    }
    return true;
}

/* Static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static uint64_t last_num = 0;   /* next integer to test */

/* Grow the cache to at least 'need' capacity. */
static void ensure_capacity(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t newcap = cache_cap ? cache_cap * 2 : 8;
    while (newcap < need)
        newcap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    /* Allocation failure handling omitted for brevity. */
    cache = tmp;
    cache_cap = newcap;
}

/* Extend the cache until it contains at least n terms. */
static void fill_cache(idx_t n)
{
    while (cache_len < n) {
        if (digits_ok(last_num)) {
            ensure_capacity(cache_len + 1);
            cache[cache_len++] = (seq_t)last_num;
        }
        ++last_num;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;    /* not defined for 0, but safe fallback */

    fill_cache(n);
    return cache[n - 1];   /* 1‑based index */
}
