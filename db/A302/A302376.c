/*
 * A302376  Number of nX3 0..1 arrays with every element equal to 1, 2, 3 or 4
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A302376: Number of n×3 0..1 arrays with every element equal to 1,2,3 or 4
   horizontally, diagonally or anti‑diagonally adjacent elements, with the upper‑left
   element zero.
   Recurrence (order 11):
   a(n) =  4*a(n-1) + 13*a(n-2) - 5*a(n-3) - 28*a(n-4) -11*a(n-5)
          +11*a(n-6) + 4*a(n-7) + 6*a(n-8) +24*a(n-9) +16*a(n-10)
          -32*a(n-11)
   initial values a(1)..a(11) are given below.  The function caches computed
   values so it works in any call order.  All arithmetic is performed in the
   integer type seq_t (which may be 8, 16, 32, 64, 128 bits or arbitrary
   precision).  No external helper functions from the utils library are
   required. */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/free_seq_array.h"

static seq_t *cache = NULL;        /* cache[i] holds a(i+1)          */
static size_t cache_len = 0;       /* number of cached elements      */

/* Recurrence coefficients and initial values. */
static const int32_t coeff[11] = {
    4, 13, -5, -28, -11, 11, 4, 6, 24, 16, -32
};

static const seq_t init_vals[11] = {
    (seq_t)1,          /* a(1) */
    (seq_t)15,         /* a(2) */
    (seq_t)76,         /* a(3) */
    (seq_t)475,        /* a(4) */
    (seq_t)2771,       /* a(5) */
    (seq_t)16451,      /* a(6) */
    (seq_t)97160,      /* a(7) */
    (seq_t)574671,     /* a(8) */
    (seq_t)3397622,    /* a(9) */
    (seq_t)20089993,   /* a(10) */
    (seq_t)118787443   /* a(11) */
};

/* Simple allocation helpers that avoid collision with the utils library. */
static seq_t *alloc_seq_array(size_t n)
{
    if (n == 0)
        return NULL;
    return (seq_t *)malloc(n * sizeof(seq_t));
}

seq_t a(idx_t n)
{
    if (n == 0)          /* sequence is defined for n ≥ 1 */
        return (seq_t)0;

    /* Initialise cache on first call. */
    if (!cache) {
        cache_len = 11;
        cache = alloc_seq_array(cache_len);
        if (!cache) abort();
        for (size_t i = 0; i < cache_len; ++i)
            cache[i] = init_vals[i];
    }

    /* Compute terms up to n using the recurrence. */
    size_t target = (size_t)n;          /* n fits into size_t for N≤10000 */
    if (cache_len < target) {
        seq_t *new_cache = alloc_seq_array(target);
        if (!new_cache) abort();
        for (size_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];
        free_seq_array(cache);
        cache = new_cache;

        /* Fill in new values. */
        for (size_t idx = cache_len; idx < target; ++idx) {
            seq_t sum = 0;
            for (size_t k = 0; k < 11; ++k) {
                int32_t c = coeff[k];
                seq_t term = cache[idx - k - 1];
                seq_t prod = (seq_t)(c >= 0 ? c : -c) * term;
                if (c >= 0)
                    sum += prod;
                else
                    sum -= prod;
            }
            cache[idx] = sum;
        }
        cache_len = target;
    }

    return cache[(size_t)n - 1];
}
