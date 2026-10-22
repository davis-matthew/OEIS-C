/*
 * A383252  Numbers that cannot be written in the form (j+2k)^2-(j+k)^2-j^2
 *          with j,k>0.
 * https://oeis.org/A383252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers that cannot be written as (j+2k)^2-(j+k)^2-j^2 with
 *            j,k>0 Characterization: a(n) consists of all positive integers
 *            that are congruent to 1,2,5,6,8,9,10,13,14 (mod 16) and have no
 *            pair of positive divisors x, y with x·y = n for which x + y is a
 *            multiple of 4. The routine generates terms in...
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
#endif

/* --------------------------------------------------------------------- */
/* Static storage for cached terms                                      */
/* --------------------------------------------------------------------- */
#ifdef __cplusplus
static std::vector<seq_t> seq_vec;
#else
static seq_t *seq_array = NULL;   /* allocated array of seq_t   */
static size_t seq_len   = 0;        /* number of stored terms     */
static size_t seq_cap   = 0;        /* allocated capacity        */
#endif

/* --------------------------------------------------------------------- */
/* Grow the C array if needed                                            */
/* --------------------------------------------------------------------- */
#ifndef __cplusplus
static inline void seq_grow(size_t need)
{
    if (seq_cap >= need) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    if (new_cap < need) new_cap = need;
    seq_t *new_ptr = (seq_t *)realloc(seq_array, new_cap * sizeof(seq_t));
    if (!new_ptr) abort();   /* allocation failed */
    seq_array = new_ptr;
    seq_cap   = new_cap;
}
#endif

/* --------------------------------------------------------------------- */
/* Small integer square‑root helper for divisor tests                    */
/* --------------------------------------------------------------------- */
static inline idx_t int_sqrt(idx_t x)
{
    idx_t r = 0;
    while ((r + 1u) * (r + 1u) <= x) ++r;
    return r;
}

/* --------------------------------------------------------------------- */
/* Check whether n has a divisor pair whose sum is a multiple of 4       */
/* --------------------------------------------------------------------- */
static inline bool has_bad_divisor(idx_t n)
{
    idx_t sq = int_sqrt(n);
    for (idx_t d = 1; d <= sq; ++d) {
        if (n % d == 0) {
            idx_t e = n / d;
            if (((d + e) & 3u) == 0u) return true;
        }
    }
    return false;
}

/* --------------------------------------------------------------------- */
/* Test whether n ≡ 1,2,5,6,8,9,10,13,14 (mod 16)                       */
/* --------------------------------------------------------------------- */
static inline bool is_candidate(idx_t cur)
{
    switch (cur & 15u) {
        case 1: case 2: case 5: case 6:
        case 8: case 9: case 10: case 13: case 14:
            return true;
        default:
            return false;
    }
}

/* --------------------------------------------------------------------- */
/* Main sequence function                                               */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined below offset; harmless */

    size_t wanted = (size_t)n;
    bool done = false;

    while (!done) {
        /* Determine next integer to test */
        idx_t cur;
#ifdef __cplusplus
        cur = seq_vec.empty() ? 1 : (idx_t)(seq_vec.back()) + 1;
#else
        cur = seq_len ? seq_array[seq_len - 1] + 1 : 1;
#endif

        /* Generate terms until we have enough cached */
        for (;;) {
            /* Check if we already have enough terms */
#ifdef __cplusplus
            if (seq_vec.size() >= wanted) { done = true; break; }
#else
            if (seq_len >= wanted) { done = true; break; }
#endif
            /* Accept the term if it satisfies both conditions */
            if (is_candidate(cur) && !has_bad_divisor(cur)) {
#ifdef __cplusplus
                seq_vec.push_back((seq_t)cur);
#else
                seq_grow(seq_len + 1);
                seq_array[seq_len++] = (seq_t)cur;
#endif
            }
            ++cur;
        }
    }

    /* Return the requested term (zero‑based index n-1) */
#ifdef __cplusplus
    return seq_vec[n - 1];
#else
    return seq_array[n - 1];
#endif
}
