/*
 * A364353  Numbers that cannot be expressed as the sum of the squares of four
 *          Fibonacci numbers.
 * https://oeis.org/A364353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   seq_alloc, seq_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A364353: numbers that cannot be expressed as the sum of the squares
   of four Fibonacci numbers.
   The implementation pre‑computes all Fibonacci squares up to a fixed
   limit, builds all sums of four of them, marks the representable numbers,
   and stores the remaining numbers in a cache that is returned on each
   call.  The cache is built only once, regardless of the order of
   successive calls to `a`. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>          /* size_t */

#include "../utils/seq_alloc.h"   /* seq_alloc(size_t) */
#include "../utils/seq_free.h"    /* seq_free(seq_t *) */

/* External integer types supplied by the build system */
typedef uint64_t idx_t;

/* Static cache for the sequence */
static seq_t   *seq_cache   = NULL;
static idx_t     seq_cache_len = 0;
static bool       seq_cache_initialized = false;

/* Build the cache on the first call */
static void init_sequence(void)
{
    /* Limit chosen to comfortably contain the first 10000 terms
       (the sequence is very dense, only a few numbers are representable). */
    const idx_t LIMIT = 2000000;

    /* ----------------------------------------------------------
       1. Generate all Fibonacci squares <= LIMIT (unique values)
       ---------------------------------------------------------- */
    seq_t fib_sqr_arr[64];   /* ample for LIMIT */
    size_t fib_cnt = 0;
    seq_t f0 = 0, f1 = 1;

    /* 0^2 */
    fib_sqr_arr[fib_cnt++] = f0 * f0;
    /* 1^2 (first occurrence of 1) */
    fib_sqr_arr[fib_cnt++] = f1 * f1;

    while (1) {
        seq_t f2 = f0 + f1;
        if (f2 * f2 > (seq_t)LIMIT) break;
        /* skip duplicate square (the second 1) */
        if (f2 * f2 != fib_sqr_arr[fib_cnt - 1]) {
            fib_sqr_arr[fib_cnt++] = f2 * f2;
        }
        f0 = f1;
        f1 = f2;
    }

    /* ----------------------------------------------------------
       2. Compute all sums of two Fibonacci squares (allowing repetition)
       ---------------------------------------------------------- */
    seq_t two_sums_arr[1024];   /* ample for fib_cnt <= 15 */
    size_t two_cnt = 0;

    for (size_t i = 0; i < fib_cnt; ++i) {
        for (size_t j = i; j < fib_cnt; ++j) {
            seq_t s = fib_sqr_arr[i] + fib_sqr_arr[j];
            if (s <= (seq_t)LIMIT) {
                two_sums_arr[two_cnt++] = s;
            }
        }
    }

    /* ----------------------------------------------------------
       3. Mark all numbers that can be expressed as a sum of four Fibonacci squares
          by iterating over pairs of two‑sums
       ---------------------------------------------------------- */
    void *rep_void = calloc((size_t)LIMIT + 1, sizeof(char));
    if (!rep_void) abort();                     /* allocation failure */
    char *rep = (char*)rep_void;                /* cast needed for C++ */

    for (size_t i = 0; i < two_cnt; ++i) {
        seq_t a = two_sums_arr[i];
        for (size_t j = 0; j < two_cnt; ++j) {
            seq_t b  = two_sums_arr[j];
            seq_t ab = a + b;
            if (ab <= (seq_t)LIMIT) {
                rep[(idx_t)ab] = 1;
            }
        }
    }

    /* ----------------------------------------------------------
       4. Build the cache of NON‑representable numbers
       ---------------------------------------------------------- */
    seq_cache = seq_alloc(LIMIT);   /* worst case capacity */
    seq_cache_len = 0;

    for (idx_t x = 1; x <= LIMIT; ++x) {
        if (!rep[x]) {
            seq_cache[seq_cache_len++] = (seq_t)x;
        }
    }

    /* ----------------------------------------------------------
       5. Clean up and mark initialized
       ---------------------------------------------------------- */
    free(rep);
    seq_cache_initialized = true;
}

/*-----------------------------------------------------------------------
   Public function required by the harness
-----------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (!seq_cache_initialized) init_sequence();

    if (n == 0 || n > seq_cache_len) return 0;   /* out of bounds */
    return seq_cache[n - 1];
}
