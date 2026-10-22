/*
 * A370688  Numbers k such that A052410(k) = A010888(k).
 * https://oeis.org/A370688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A370688: 
 * a(n) is the nth smallest non‑negative integer k with A052410(k)=A010888(k).
 * The sequence consists of 0, 1, 3, 6 and all numbers of the form 
 * 2^(6*m+1), 5^(6*m+1), or 7^(3*m+1)  (m ≥ 0). 
 * This implementation generates the terms on demand by merging the three
 * geometric progressions together with the two constants 3 and 6. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mul_small.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

/* Current candidates from the three series */
static seq_t next2 = (seq_t)0;
static seq_t next5 = (seq_t)0;
static seq_t next7 = (seq_t)0;

/* Has the constant 3 been output yet? */
static bool const3_done = false;
/* Has the constant 6 been output yet? */
static bool const6_done = false;

seq_t a(idx_t n)
{
    if (n == 0)   /* offset protection – should not happen in OEIS */
        return (seq_t)0;

    /* initialise on first call */
    if (cache == NULL)
    {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();
        cache[0] = (seq_t)0;   /* a(1) */
        cache[1] = (seq_t)1;   /* a(2) */
        cache_len = 2;

        /* initialise series */
        next2 = (seq_t)2;   /* 2^(6*0+1)   */
        next5 = (seq_t)5;   /* 5^(6*0+1)   */
        next7 = (seq_t)7;   /* 7^(3*0+1)   */

        const3_done = false;
        const6_done = false;
    }

    /* generate terms until we reach the requested index */
    while (cache_len < n)
    {
        seq_t val;
        int type = -1;   /* 0 = const 3, 1 = const 6, 2 = series 2,
                            3 = series 5, 4 = series 7 */

        /* candidate constant 3 */
        if (!const3_done) { val = (seq_t)3; type = 0; }

        /* candidate constant 6 */
        if (!const6_done) {
            seq_t v = (seq_t)6;
            if (type == -1 || v < val) { val = v; type = 1; }
        }

        /* candidate from 2^ series (factor 64) */
        {
            seq_t v = next2;
            if (type == -1 || v < val) { val = v; type = 2; }
        }

        /* candidate from 5^ series (factor 15625) */
        {
            seq_t v = next5;
            if (v < val) { val = v; type = 3; }
        }

        /* candidate from 7^ series (factor 343) */
        {
            seq_t v = next7;
            if (v < val) { val = v; type = 4; }
        }

        /* ensure space in cache */
        if (cache_len == cache_cap)
        {
            cache_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (tmp == NULL) { free(cache); abort(); }
            cache = tmp;
        }

        cache[cache_len++] = val;

        /* advance the source of the chosen term */
        switch (type)
        {
            case 0: const3_done = true; break;
            case 1: const6_done = true; break;
            case 2: next2 = mul_small(next2, 64);  break;   /* 2^6 = 64 */
            case 3: next5 = mul_small(next5, 15625); break; /* 5^6 = 15625 */
            case 4: next7 = mul_small(next7, 343);   break; /* 7^3 = 343  */
        }
    }

    return cache[n - 1];
}
