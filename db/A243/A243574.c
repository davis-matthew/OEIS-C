/*
 * A243574  Irregular triangular array of denominators of the positive
 *          rational numbers ordered as in Comments.
 * https://oeis.org/A243574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf, frac
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 671, u32 n <= 6000, u64 n <= 6000,
 *            u128 n <= 6000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence implementation of OEIS A243574
 *   Row 1 = { 1/1 }
 *   Row 2 = { 1/2 }
 *   For n ≥ 3, row n consists of the fractions in increasing order
 *   generated from the previous rows as follows:
 *     1/(x + 1)  for each x in row n‑1
 *   together with
 *     x + 1       for each x in row n‑2
 *   The sequence a(n) is the denominator of the nth element when all rows
 *   are concatenated in order.                                            */

#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/* Helper structure and functions                                        */
/* --------------------------------------------------------------------- */

typedef struct {          /* rational number in lowest terms */
    seq_t num;
    seq_t den;
} Frac;

/* Comparator: order Fractions by their numeric value a / b. */
static int cmp_frac(const void *p1, const void *p2)
{
    const Frac *f1 = (const Frac *)p1;
    const Frac *f2 = (const Frac *)p2;

    seq_t left  = f1->num * f2->den;
    seq_t right = f2->num * f1->den;

    if (left < right)  return -1;
    if (left > right)  return  1;
    return 0;
}

/* Ensure the denominator cache can hold at least n elements. */
static void ensure_cache_capacity(size_t n)
{
    static seq_t *den_cache = NULL;
    static size_t cache_len = 0;

    if (den_cache == NULL || n > cache_len) {
        size_t new_cap = (cache_len == 0) ? 16 : cache_len * 2;
        while (new_cap < n) new_cap *= 2;
        den_cache = (seq_t *)realloc(den_cache, new_cap * sizeof(seq_t));
        cache_len = new_cap;
    }
    *(&den_cache) = den_cache;
}

/* --------------------------------------------------------------------- */
/* State to generate rows on demand or cache results                    */
/* --------------------------------------------------------------------- */
static seq_t *den_cache = NULL;        /* global cache of denominators           */
static size_t cache_len = 0;           /* allocated capacity for den_cache      */
static size_t total_terms = 0;         /* number of terms produced so far       */

static Frac *row_prev1 = NULL;         /* most recent row (row n‑1)             */
static size_t len_prev1 = 0;
static Frac *row_prev2 = NULL;         /* second most recent row (row n‑2)      */
static size_t len_prev2 = 0;

/* Allocate / grow the cache if necessary. */
static inline void ensure_cache_capacity_local(size_t n)
{
    if (den_cache == NULL || n > cache_len) {
        size_t new_cap = (cache_len == 0) ? 16 : cache_len * 2;
        while (new_cap < n) new_cap *= 2;
        den_cache = (seq_t *)realloc(den_cache, new_cap * sizeof(seq_t));
        cache_len = new_cap;
    }
}

/* --------------------------------------------------------------------- */
/* Main routine to generate all rows until we have at least n terms      */
/* --------------------------------------------------------------------- */
static void generate_up_to(size_t target)
{
    /* Initial rows only once. */
    if (total_terms == 0) {
        /* Row 1: 1/1 */
        row_prev2 = (Frac *)malloc(sizeof(Frac));
        row_prev2[0].num = (seq_t)1;
        row_prev2[0].den = (seq_t)1;
        len_prev2   = 1;
        ensure_cache_capacity_local(1);
        den_cache[0] = (seq_t)1;
        total_terms = 1;

        /* Row 2: 1/2 */
        row_prev1 = (Frac *)malloc(sizeof(Frac));
        row_prev1[0].num = (seq_t)1;
        row_prev1[0].den = (seq_t)2;
        len_prev1   = 1;
        ensure_cache_capacity_local(2);
        den_cache[1] = (seq_t)2;
        total_terms = 2;
    }

    /* Generate rows until we reach the requested amount. */
    while (total_terms < target) {
        size_t new_len = len_prev1 + len_prev2;
        Frac *cur = (Frac *)malloc(new_len * sizeof(Frac));

        size_t idx = 0;

        /* 1/(x + 1) for each x in row n‑1 (row_prev1). */
        for (size_t i = 0; i < len_prev1; ++i) {
            seq_t a = row_prev1[i].num;
            seq_t b = row_prev1[i].den;
            cur[idx].num = b;
            cur[idx].den = a + b;
            ++idx;
        }

        /* x + 1 for each x in row n‑2 (row_prev2). */
        for (size_t i = 0; i < len_prev2; ++i) {
            seq_t a = row_prev2[i].num;
            seq_t b = row_prev2[i].den;
            cur[idx].num = a + b;
            cur[idx].den = b;
            ++idx;
        }

        /* Ensure we have generated the correct number of elements. */
        /* (idx should equal new_len) */

        /* Sort the new row by the numerical value of the fractions. */
        qsort(cur, new_len, sizeof(Frac), cmp_frac);

        /* Append denominators to the cache. */
        ensure_cache_capacity_local(total_terms + new_len);
        for (size_t i = 0; i < new_len; ++i)
            den_cache[total_terms + i] = cur[i].den;

        total_terms += new_len;

        /* Shift rows for the next iteration. */
        free(row_prev2);
        row_prev2 = row_prev1;
        len_prev2 = len_prev1;

        row_prev1 = cur;
        len_prev1 = new_len;
    }
}

/* --------------------------------------------------------------------- */
/* Public API – sequence a(n)                                            */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, graceful fallback */

    /* Make sure we have generated up to index n. */
    size_t idx = (size_t)n;
    if (idx > total_terms)
        generate_up_to(idx);

    return den_cache[idx - 1];
}
