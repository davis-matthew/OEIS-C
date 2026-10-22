/*
 * A319422  Records in A175046.
 * https://oeis.org/A319422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 20, u32 n <= 72, u64 n <= 272,
 *            u128 n <= 1056, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A319422: the sequence of integers whose binary
   representation is an alternating string of 11's and 00's, with
   optionally one of the 00 blocks replaced by 000.  The sequence
   is generated lazily and cached in increasing order. */

#include <stdlib.h>

/* Cached state */
static seq_t *cache = NULL;      /* array of generated values      */
static size_t cache_len = 0;     /* current number of values       */
static size_t cache_cap = 0;     /* current allocated capacity     */
static idx_t max_L = 0;          /* largest L already generated    */

/* Ensure the cache can hold at least `new_needed` elements. */
static void ensure_cache_capacity(size_t new_needed)
{
    if (new_needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < new_needed)
        new_cap *= 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Append a new value to the cache. */
static void push_to_cache(seq_t val)
{
    ensure_cache_capacity(cache_len + 1);
    cache[cache_len++] = val;
}

/* Generate all numbers for a given L (number of “11” blocks).
   They are appended to the cache in strictly increasing order. */
static void generate_L(idx_t L)
{
    seq_t val, v;
    idx_t i, zero_pos;

    /* ----- Case 1: ends with “11” (no replacement of 00) ----- */
    val = 0;
    val = (val << 2) | (seq_t)3;          /* first “11” */
    for (i = 1; i < L; ++i) {
        val = val << 2;                   /* “00”   */
        val = (val << 2) | (seq_t)3;      /* “11”   */
    }
    push_to_cache(val);

    /* Replacements of one 00 by 000 */
    for (zero_pos = 1; zero_pos < L; ++zero_pos) {
        v = 0;
        v = (v << 2) | (seq_t)3;          /* first “11” */
        for (i = 1; i < L; ++i) {
            if (i == zero_pos)
                v <<= 3;                  /* “000” */
            else
                v <<= 2;                  /* “00”  */
            v = (v << 2) | (seq_t)3;      /* “11”  */
        }
        push_to_cache(v);
    }

    /* ----- Case 2: ends with “00” (no replacement of 00) ----- */
    val = 0;
    val = (val << 2) | (seq_t)3;          /* first “11” */
    for (i = 1; i <= L; ++i) {
        val = val << 2;                   /* “00”   */
        if (i < L)
            val = (val << 2) | (seq_t)3;  /* “11”   */
    }
    push_to_cache(val);

    /* Replacements of one 00 by 000 */
    for (zero_pos = 1; zero_pos <= L; ++zero_pos) {
        v = 0;
        v = (v << 2) | (seq_t)3;          /* first “11” */
        for (i = 1; i <= L; ++i) {
            if (i == zero_pos)
                v <<= 3;                  /* “000” */
            else
                v <<= 2;                  /* “00”  */
            if (i < L)
                v = (v << 2) | (seq_t)3;  /* “11”  */
        }
        push_to_cache(v);
    }
}

/* --------------------------------------------------------------------- */
/* Public sequence function                                               */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        ++max_L;               /* increase L and generate all new terms */
        generate_L(max_L);
    }

    /* The values are already produced in ascending order, so
       the array is sorted.  Return the requested term. */
    return cache[n - 1];
}
