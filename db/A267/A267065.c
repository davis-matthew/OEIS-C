/*
 * A267065  Coefficient of x^6 in the minimal polynomial of the continued
 *          fraction [1^n,sqrt(2)+sqrt(3),1,1,...], where 1^n means n ones.
 * https://oeis.org/A267065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 0, i16 n <= 2, i32 n <= 4, i64 n <= 10, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A267065:  coefficient of x^6 in the minimal polynomial of
   [1^n, √2+√3, 1,1,....].
   For n ≥ 9 the sequence satisfies the linear recurrence
        a(n) = 34 a(n-1) + 714 a(n-2) – 4641 a(n-3) – 12376 a(n-4)
               + 12376 a(n-5) + 4641 a(n-6) – 714 a(n-7) – 34 a(n-8) + a(n-9)
   Initial values:
        a(0)=−18, a(1)=2498, a(2)=7790, a(3)=1588998,
        a(4)=47783370, a(5)=2692503902, a(6)=118222343438,
        a(7)=5700687414690, a(8)=265166547527598. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* static cache for memoisation */
static seq_t *cache = NULL;      /* dynamically allocated array of terms */
static size_t cache_cap = 0;      /* current allocated capacity */
static size_t cache_size = 0;     /* number of terms stored (last index +1) */
static bool initialized = false;

/* initialise cache with the 9 known start values */
static void init_cache(void)
{
    if (initialized) return;

    cache_cap = 16;   /* can hold at least 9 terms */
    cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    if (!cache) abort();  /* abort if out of memory */

    /* initial 9 terms – these are part of the definition */
    cache[0] = (seq_t)-18;
    cache[1] = (seq_t)2498;
    cache[2] = (seq_t)7790;
    cache[3] = (seq_t)1588998;
    cache[4] = (seq_t)47783370;
    cache[5] = (seq_t)2692503902;
    cache[6] = (seq_t)118222343438;
    cache[7] = (seq_t)5700687414690;
    cache[8] = (seq_t)265166547527598;
    cache_size = 9;

    initialized = true;
}

/* enlarge cache so that it can hold at least indices 0..need_index */
static void grow_cache(size_t need_index)
{
    if (cache_cap > need_index) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap <= need_index) new_cap <<= 1;
    seq_t *new_buf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_buf) abort();
    cache = new_buf;
    cache_cap = new_cap;
}

/* Return the n‑th term of the sequence. */
seq_t a(idx_t n)
{
    if (!initialized) init_cache();

    /* n is non‑negative; cast to size_t safely */
    size_t idx = (size_t)n;
    if (idx < cache_size) return cache[idx];

    /* need terms up to idx */
    grow_cache(idx);
    for (size_t i = cache_size; i <= idx; ++i) {
        seq_t val =
            (seq_t)34   * cache[i-1] +
            (seq_t)714  * cache[i-2] -
            (seq_t)4641 * cache[i-3] -
            (seq_t)12376 * cache[i-4] +
            (seq_t)12376 * cache[i-5] +
            (seq_t)4641 * cache[i-6] -
            (seq_t)714  * cache[i-7] -
            (seq_t)34   * cache[i-8] +
            cache[i-9];
        cache[i] = val;
    }
    cache_size = idx + 1;
    return cache[idx];
}
