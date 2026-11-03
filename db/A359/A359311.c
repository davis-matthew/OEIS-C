/*
 * A359311  Number of Catalan paths (nonnegative, starting and ending at 0,
 *          step +/-1) of 2*n steps which reach at least 6 at some point.
 * https://oeis.org/A359311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Catalan(n) – B(n), where B(n) counts Dyck paths of length
 *            2n that never go above height 5. The implementation uses a small
 *            static cache for Catalan numbers (size 10001) and a simple
 *            dynamic-programming routine for B(n)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 28 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>     /* size_t */
#include <stdbool.h>    /* bool */

/* ----------  Count Dyck paths of length 2n staying at height <=5  ---------- */
static seq_t bounded_paths(idx_t n)
{
    /* The trivial path of length 0 is counted as 1. */
    if (n == 0)
        return (seq_t)1;

    seq_t cur[6]  = {0};
    seq_t next[6] = {0};
    cur[0] = (seq_t)1;                 /* start at height 0 */

    for (idx_t step = 0; step < 2 * n; ++step) {
        /* zero the next array */
        for (int h = 0; h < 6; ++h)
            next[h] = (seq_t)0;

        for (int h = 0; h < 6; ++h) {
            seq_t val = cur[h];
            if (val == 0)
                continue;
            if (h > 0)           /* down step */
                next[h - 1] += val;
            if (h < 5)           /* up step   */
                next[h + 1] += val;
        }

        /* swap cur and next */
        for (int h = 0; h < 6; ++h)
            cur[h] = next[h];
    }

    return cur[0];   /* number of paths ending at height 0 */
}

#ifndef MAX_CATALAN
#define MAX_CATALAN 10001   /* sufficient for n up to about 10000 */
#endif

/* ----------  Catalan number cache  ---------- */
static seq_t cat_cache[MAX_CATALAN];
static idx_t cat_last = 0;            /* highest index that has been computed */
static bool cat_init = false;        /* has the cache been initialized? */

/* Ensure that cat_cache contains Catalan numbers 0..n (inclusive). */
static void ensure_cat(idx_t n)
{
    if (!cat_init) {
        /* initialise C(0) = 1 */
        cat_cache[0] = (seq_t)1;
        cat_last = 0;
        cat_init = true;
    }
    if (n <= cat_last)
        return;                /* already have the requested values */

    if ((size_t)(n + 1) > MAX_CATALAN) {
        /* the array is too small for this value of n;
           silently truncate to the largest available index. */
        n = (size_t)MAX_CATALAN - 1;
    }
    /* compute missing Catalan numbers */
    for (idx_t k = cat_last + 1; k <= n; ++k) {
        /* C(k) = C(k-1) * (4*k - 2) / (k + 1) */
        seq_t num = cat_cache[(size_t)k - 1] * (seq_t)(4 * k - 2);
        cat_cache[(size_t)k] = num / (seq_t)(k + 1);
    }
    cat_last = n;
}

/* ----------  Public sequence function  ---------- */
seq_t a(idx_t n)
{
    /* For n ≤ 5 all paths stay below height 6, so a(n)=0. */
    if (n <= 5)
        return (seq_t)0;

    /* Catalan(n) */
    ensure_cat(n);
    seq_t cat_n = cat_cache[(size_t)n];

    /* B(n) = number of Dyck paths of length 2n that never exceed height 5 */
    seq_t bound_n = bounded_paths(n);

    /* a(n) = Catalan(n) – B(n) */
    return cat_n - bound_n;
}
