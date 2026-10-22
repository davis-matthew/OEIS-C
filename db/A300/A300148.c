/*
 * A300148  Trajectory of the "Erase or triple" protocol applied to 1 (see
 *          Comments section for how the protocol works).
 * https://oeis.org/A300148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; a(n+1)=next(a(n)) where next is the 
   “Erase or triple” protocol applied to a(n). */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Helper: compute the next value of the protocol */
static seq_t next_value(seq_t x)
{
    /* If x is 0, it has no duplicate digits; triple it. */
    if (x == 0)
        return 0;          /* 0 triples to 0 and will stay 0 */

    /* Count the number of decimal digits in x */
    size_t cnt_digits;
    if (x == 0)
        cnt_digits = 1;
    else {
        seq_t tmp = x;
        cnt_digits = 0;
        while (tmp > 0) {
            cnt_digits++;
            tmp /= 10;
        }
    }

    /* Store the digits in their natural order (most significant first) */
    unsigned char *digits = (unsigned char*)malloc(cnt_digits);
    if (!digits)    /* unlikely, but fall back to triple */
        return x * 3;

    seq_t tmp = x;
    for (size_t i = 0; i < cnt_digits; ++i) {
        digits[cnt_digits - 1 - i] = (unsigned char)(tmp % 10);
        tmp /= 10;
    }

    unsigned char cnt[10] = {0};
    for (size_t i = 0; i < cnt_digits; ++i)
        cnt[digits[i]]++;

    /* Check for duplicate digits */
    bool has_dup = false;
    for (int d = 0; d < 10; ++d)
        if (cnt[d] > 1) { has_dup = true; break; }

    if (has_dup) {
        /* Erase all digits that appear more than once */
        seq_t res = 0;
        for (size_t i = 0; i < cnt_digits; ++i)
            if (cnt[digits[i]] == 1)
                res = res * 10 + digits[i];
        free(digits);
        return res;        /* result may be 0 if no unique digits */
    } else {
        free(digits);
        return x * 3;      /* triple if no duplicates */
    }
}

/* Main sequence function with memoisation */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined below offset, return 0 as guard */

    /* Static cache */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Initialise cache with first term if needed */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 1;          /* a(1) = 1 */
        cache_len = 1;
    }

    /* Extend cache up to requested index n */
    if (cache_len < n) {
        /* Ensure capacity */
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n)
            new_cap *= 2;
        if (new_cap > cache_cap) {
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp) {
                cache = tmp;
                cache_cap = new_cap;
            } else {
                /* Allocation failed; fall back to recomputation each call */
                cache_len = 0;
                free(cache);
                cache = NULL;
                cache_cap = 0;
                /* Recompute by naive method */
                seq_t val = 1;
                for (idx_t i = 1; i < n; ++i)
                    val = next_value(val);
                return val;
            }
        }

        /* Compute missing terms */
        for (size_t i = cache_len; i < (size_t)n; ++i)
            cache[i] = next_value(cache[i - 1]);

        cache_len = (size_t)n;
    }

    return cache[n - 1];
}
