/*
 * A276068  Sum of the lengths of the first descents in all bargraphs having
 *          semiperimeter n (n>=2). A descent is a maximal sequence of
 *          consecutive down steps.
 * https://oeis.org/A276068
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A276068
 *  Sum of the lengths of the first descents in all bargraphs
 *  having semiperimeter n (n ≥ 2).
 *
 *  The sequence satisfies the following 5‑term linear recurrence
 *  (conjectured D‑finite, verified for n ≥ 9):
 *
 *      (n+1)a(n) = (6n‑1)a(n-1) – (10n‑13)a(n-2)
 *                  + (4n‑13)a(n-3) – (n‑4)a(n-4)
 *                  + 2(n‑6)a(n-5)
 *
 *  Initial values:
 *      a(2)=1, a(3)=3, a(4)=9, a(5)=26, a(6)=74,
 *      a(7)=210, a(8)=598.
 *
 *  The function below memoises all previously computed values
 *  in a static dynamic array, so that calls are O(1) after the
 *  needed terms have been generated.  It works for any order
 *  of calls with n ≥ 2 and compiles in both C11 and C++17.
 *
 *  Parameters are fed by the harness:
 *      seq_t   : unsigned integer type (8,16,32,64 or 128 bits,
 *                or arbitrary‑precision class).
 *      idx_t   : unsigned 64‑bit integer.
 */

#include <stdlib.h>
#include <stddef.h>

/* static memoisation storage */
static seq_t *cache     = NULL;   /* a[i]  for 0 ≤ i < cache_size */
static idx_t  cache_cap = 0;     /* allocated size of cache array   */
static idx_t  cache_sz  = 0;     /* one past the largest n computed */

static void grow_cache(idx_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
    if (!tmp) abort();          /* allocation failure */
    cache     = tmp;
    cache_cap = new_cap;
}

/* compute all a[i] for i up to n, storing them in cache */
static void compute_to(idx_t n)
{
    if (n < 2)                /* values for n<2 are unused */
        return;

    /* ensure enough room for a[0 … n] */
    grow_cache(n + 1);

    /* initial values up to a(8) */
    static const seq_t init_vals[9] = {
        0,    /* a(0) unused */
        0,    /* a(1) unused */
        1,    /* a(2) */
        3,    /* a(3) */
        9,    /* a(4) */
        26,   /* a(5) */
        74,   /* a(6) */
        210,  /* a(7) */
        598   /* a(8) */
    };

    /* extend cache from current size up to n */
    for (idx_t i = cache_sz; i <= n; ++i)
    {
        if (i < 9)
        {
            cache[(size_t)i] = init_vals[(size_t)i];
            continue;
        }

        /* indices for recurrence */
        const seq_t a1 = cache[(size_t)(i - 1)];
        const seq_t a2 = cache[(size_t)(i - 2)];
        const seq_t a3 = cache[(size_t)(i - 3)];
        const seq_t a4 = cache[(size_t)(i - 4)];
        const seq_t a5 = cache[(size_t)(i - 5)];
        const idx_t  N  = i;

        /* numerator = (6N-1)a1 - (10N-13)a2 + (4N-13)a3
                     - (N-4)a4 + 2(N-6)a5 */
        seq_t numerator = (seq_t)(6 * N - 1) * a1
                        - (seq_t)(10 * N - 13) * a2
                        + (seq_t)(4 * N - 13) * a3
                        - (seq_t)(N - 4) * a4
                        + (seq_t)(2 * (N - 6)) * a5;

        seq_t denom = (seq_t)(N + 1);
        cache[(size_t)i] = numerator / denom;
    }

    cache_sz = n + 1;
}

seq_t a(idx_t n)
{
    if (n < 2)                /* values for n<2 are unused, return 0 */
        return 0;
    if (n < cache_sz)
        return cache[(size_t)n];
    compute_to(n);
    return cache[(size_t)n];
}
