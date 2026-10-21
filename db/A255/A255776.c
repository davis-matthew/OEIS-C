/*
 * A255776  Number of (n+2)X(1+2) 0..1 arrays with no 3x3 subblock diagonal
 *          sum 1 and no antidiagonal sum 1 and no row sum 0 and no column sum
 *          0.
 * https://oeis.org/A255776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 15, u64 n <= 33, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A255776: Number of (n+2)×(1+2) 0..1 arrays with no 3×3 subblock
 * diagonal sum 1, no antidiagonal sum 1, no zero row or column.
 *
 * The sequence satisfies an 18‑term linear recurrence (valid for n > 18)
 *
 *   a(n) =  2·a(n-1) + 4·a(n-2) + 3·a(n-3) +  1·a(n-4)
 *           -7·a(n-5) -6·a(n-6) -20·a(n-7) -16·a(n-8)
 *           -25·a(n-9) -25·a(n-10) -20·a(n-11) -21·a(n-12)
 *           -7·a(n-13) -6·a(n-14) +4·a(n-15) +4·a(n-16)
 *           +4·a(n-17) +2·a(n-18)
 *
 * Initial values (n = 1 … 18)
 *
 *     140, 436, 1512, 5126, 17325, 58707, 198635, 672183,
 *     2274999, 7699081, 26056081, 88181805, 298433705,
 *     1009989977, 3418112157, 11567925109, 39149360907,
 *     132493286347
 *
 * The implementation uses a static cache so that calls with arbitrary
 * order and size are handled efficiently.  All arithmetic is performed
 * with the type seq_t, as required by the build system.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Forward declaration: seq_t and idx_t are supplied by the build system. */

/* -------------------------------------------------------------------- */
/* Internal helpers:   Resize cache if needed.                           */
/* -------------------------------------------------------------------- */
static void
ensure_cache(idx_t needed, size_t *capp, seq_t **ptrp)
{
    size_t needed_cnt = (size_t)(needed + 1);          /* include unused cache[0] */
    if (*capp < needed_cnt) {
        size_t new_cap = needed_cnt;
        seq_t *tmp = (seq_t *)realloc(*ptrp, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* In a real library we would handle the error;  here
               we just abort to keep the example minimal. */
            abort();
        }
        *ptrp = tmp;
        *capp = new_cap;
    }
}

/* -------------------------------------------------------------------- */
/* Cache for the sequence values.  Index i stores a(i).                 */
/* -------------------------------------------------------------------- */
static seq_t *cache    = NULL;     /* 1‑based, cache[0] unused */
static size_t cache_cap = 0;       /* actual allocated size */
static idx_t   cache_max = 0;      /* highest n that has been filled */

/* -------------------------------------------------------------------- */
/* Sequence constants.  First 18 terms and the recurrence coefficients. */
/* -------------------------------------------------------------------- */
static const seq_t init_18[18] = {
    (seq_t)140ULL,
    (seq_t)436ULL,
    (seq_t)1512ULL,
    (seq_t)5126ULL,
    (seq_t)17325ULL,
    (seq_t)58707ULL,
    (seq_t)198635ULL,
    (seq_t)672183ULL,
    (seq_t)2274999ULL,
    (seq_t)7699081ULL,
    (seq_t)26056081ULL,
    (seq_t)88181805ULL,
    (seq_t)298433705ULL,
    (seq_t)1009989977ULL,
    (seq_t)3418112157ULL,
    (seq_t)11567925109ULL,
    (seq_t)39149360907ULL,
    (seq_t)132493286347ULL
};

static const int coeff[18] = {
    2, 4, 3, 1, -7, -6, -20, -16,
   -25, -25, -20, -21, -7, -6, 4, 4, 4, 2
};

/* -------------------------------------------------------------------- */
/* Public function: compute a(n).                                       */
/* -------------------------------------------------------------------- */
seq_t
a(idx_t n)
{
    /*  Direct lookup for the first 18 terms.  The recurrence is
        only defined for n > 18. */
    if (n <= 18)
        return init_18[n - 1];

    /* Ensure the cache can hold indices up to n. */
    ensure_cache(n, &cache_cap, &cache);

    /* Initialise the cache with the first 18 terms if necessary. */
    if (cache_max < 18) {
        for (idx_t i = 1; i <= 18; ++i)
            cache[i] = init_18[i - 1];
        cache_max = 18;
    }

    /* Compute all missing values up to n using the recurrence. */
    if (n > cache_max) {
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            seq_t sum = 0;
            /* Sum over the 18 previous terms with the appropriate
               coefficients.  The recurrence is
               a(i) = Σ coeff[k] * a(i-k-1). */
            for (int k = 0; k < 18; ++k) {
                seq_t term = cache[i - k - 1];   /* a(i-(k+1)) */
                int c = coeff[k];
                if (c >= 0)
                    sum += (seq_t)c * term;
                else
                    sum -= (seq_t)(-c) * term;
            }
            cache[i] = sum;
        }
        cache_max = n;
    }

    return cache[n];
}
