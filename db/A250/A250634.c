/*
 * A250634  Number of (2+1)X(n+1) 0..2 arrays with nondecreasing
 *          x(i,j)+x(i,j-1) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 30, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A250634: number of (2+1)×(n+1) 0..2 arrays with the
 * described monotone constraints.
 *
 * The sequence satisfies the following order‑13 linear recurrence for
 * all n > 16:
 *   a(n) = 9*a(n-1) -21*a(n-2) -32*a(n-3) +195*a(n-4) -160*a(n-5)
 *          -371*a(n-6) +679*a(n-7) +5*a(n-8) -716*a(n-9) +413*a(n-10)
 *          +152*a(n-11) -200*a(n-12) +48*a(n-13)
 *
 * Initial values (1 ≤ n ≤ 16):
 *   125, 654, 2969, 12360, 49310, 190213, 720347, 2690076,
 *   9964237, 36686400, 134581437, 492406440, 1798700175,
 *   6562904559, 23928907661, 87203461550
 *
 * The implementation memoises the computed terms so that any order of
 * calls is handled efficiently.
 */

#include <stddef.h>
#include <stdlib.h>

/* forward declaration of the helper that ensures cache capacity */
static void ensure_capacity(idx_t need, seq_t **p, size_t *cap);

/* static cache of computed values (index 0 is unused). */
static seq_t *cache = NULL;
static size_t cap   = 0;   /* allocated capacity (including index 0) */
static size_t used  = 0;   /* highest computed index */

/*
 * Ensure that the cache can hold indices up to 'need'.
 * The cache is reallocated using realloc as needed.
 */
static void ensure_capacity(idx_t need, seq_t **p, size_t *current_cap)
{
    size_t required = (size_t)need + 1;          /* +1 because index 0 is unused */
    if (*current_cap >= required)
        return;

    size_t new_cap = *current_cap ? (*current_cap << 1) : 64;
    while (new_cap < required)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(*p, new_cap * sizeof(seq_t));
    if (tmp == NULL) {
        /* allocation failure; terminate the program (safe for this context). */
        exit(EXIT_FAILURE);
    }
    *p = tmp;
    *current_cap = new_cap;
}

/*
 * Main entry point required by the tests.
 * Returns a(n) for n ≥ 1. For n < 1 returns 0.
 */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n=0, but safe to return 0 */

    /* initialise cache once with the first 16 terms */
    if (!cache) {
        ensure_capacity(16, &cache, &cap);
        static const seq_t init_vals[17] = {
            0,                     /* index 0 unused */
            125ULL,                /* a(1) */
            654ULL,                /* a(2) */
            2969ULL,               /* a(3) */
            12360ULL,              /* a(4) */
            49310ULL,              /* a(5) */
            190213ULL,             /* a(6) */
            720347ULL,             /* a(7) */
            2690076ULL,            /* a(8) */
            9964237ULL,            /* a(9) */
            36686400ULL,           /* a(10) */
            134581437ULL,          /* a(11) */
            492406440ULL,          /* a(12) */
            1798700175ULL,         /* a(13) */
            6562904559ULL,         /* a(14) */
            23928907661ULL,        /* a(15) */
            87203461550ULL         /* a(16) */
        };
        for (size_t i = 1; i <= 16; ++i)
            cache[i] = init_vals[i];
        used = 16;
    }

    /* if the requested value is already computed, return it */
    if ((size_t)n <= used)
        return cache[n];

    /* extend the cache and compute terms up to n */
    ensure_capacity(n, &cache, &cap);
    for (size_t k = used + 1; k <= (size_t)n; ++k) {
        /* compute a(k) using the recurrence */
        seq_t val = cache[k-1] * 9;
        val += cache[k-4] * 195;
        val += cache[k-7] * 679;
        val += cache[k-8] * 5;
        val += cache[k-10] * 413;
        val += cache[k-11] * 152;
        val += cache[k-13] * 48;

        val -= cache[k-2] * 21;
        val -= cache[k-3] * 32;
        val -= cache[k-5] * 160;
        val -= cache[k-6] * 371;
        val -= cache[k-9] * 716;
        val -= cache[k-12] * 200;

        cache[k] = val;
    }

    used = (size_t)n;
    return cache[n];
}
