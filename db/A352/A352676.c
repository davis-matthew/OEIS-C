/*
 * A352676  Intersection of Beatty sequences for sqrt(3) and 1+sqrt(3).
 * https://oeis.org/A352676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Intersection of Beatty sequences for sqrt(3) and 1+sqrt(3).  */
/*  The value a(n) is the n‑th integer that satisfies            */
/*  floor(k*sqrt(3)) = floor(l*(1+sqrt(3))) for some positive   */
/*  integers k,l.  The two sequences are                            */
/*      A: floor(k*sqrt(3))                                */
/*      B: k + floor(k*sqrt(3))                            */
/*  The algorithm generates the two increasing sequences A and B  */
/*  with a two‑pointer technique and records the common values.   */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Integer square root: floor(sqrt(x)) for any unsigned integer size. */
static seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = x;
    while (lo <= hi) {
        seq_t mid = lo + (hi - lo) / 2;
        if (mid == 0) {                /* avoid division by zero */
            lo = 1;
            continue;
        }
        if (mid > x / mid) {            /* mid*mid > x */
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }
    return hi;
}

/* Grow a dynamic array to at least 'need' elements. */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t newcap = (*cap == 0) ? 4 : *cap;
        while (newcap < need)
            newcap <<= 1;
        *arr = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
        *cap = newcap;
    }
}

/* Return the n-th term of OEIS A352676. */
seq_t a(idx_t n)
{
    /* Static state for memoisation. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    static idx_t i1 = 1;                /* index in first Beatty sequence */
    static idx_t i2 = 1;                /* index in second Beatty sequence */
    static seq_t v1 = 0;                /* current value floor(i1*sqrt(3))   */
    static seq_t v2 = 0;                /* current value i2 + floor(i2*sqrt(3)) */
    static bool initialized = false;

    if (!initialized) {
        v1 = floor_sqrt((seq_t)3 * (seq_t)i1 * (seq_t)i1);
        v2 = i2 + floor_sqrt((seq_t)3 * (seq_t)i2 * (seq_t)i2);
        initialized = true;
    }

    /* Generate intersection values until we have at least n cached. */
    while (cache_len < n) {
        if (v1 < v2) {
            ++i1;
            v1 = floor_sqrt((seq_t)3 * (seq_t)i1 * (seq_t)i1);
        } else if (v1 > v2) {
            ++i2;
            v2 = i2 + floor_sqrt((seq_t)3 * (seq_t)i2 * (seq_t)i2);
        } else { /* v1 == v2: a common element */
            grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = v1;     /* equal to v2 */
            ++i1;
            v1 = floor_sqrt((seq_t)3 * (seq_t)i1 * (seq_t)i1);
            ++i2;
            v2 = i2 + floor_sqrt((seq_t)3 * (seq_t)i2 * (seq_t)i2);
        }
    }

    return cache[n - 1];
}
