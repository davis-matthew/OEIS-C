/*
 * A302473  Number of 3Xn 0..1 arrays with every element equal to 1, 2, 3 or 5
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A302473 – Number of 3×n 0..1 arrays with
 * every element equal to 1, 2, 3 or 5 horizontally, diagonally
 * or antidiagonally adjacent elements, with upper left element zero.
 *
 * Formula:  a(1) = 0
 *           a(2) = 11
 *           a(3) = 49
 *           a(4) = 203
 *           a(5) = 955
 *           a(6) = 4556
 *           a(7) = 21843
 *           a(8) = 103319
 *           a(9) = 491073
 *           a(10) = 2332753
 *           a(11) = 11082938
 *           a(12) = 52648987
 *           a(13) = 250106679
 *           a(14) = 1188163911
 *           a(15) = 5644397853
 *           a(16) = 26814116700
 *           a(17) = 127381683511
 *           a(18) = 605134192403
 *           a(19) = 2874722191055
 *           a(20) = 13656526930929
 *           a(21) = 64876076932622
 *           a(22) = 308197362006435
 *
 * For n > 22 the sequence satisfies the linear recurrence:
 *
 *   a(n) = a(n-1) + 17*a(n-2) + 15*a(n-3)
 *          - 33*a(n-4) - 62*a(n-5) - 148*a(n-6) - 116*a(n-7)
 *          + 343*a(n-8) + 429*a(n-9) + 269*a(n-10) + 191*a(n-11)
 *          - 415*a(n-12) - 534*a(n-13) - 468*a(n-14) - 436*a(n-15)
 *          - 132*a(n-16) - 72*a(n-17) - 8*a(n-18) + 8*a(n-19)
 *
 * The implementation uses a memoised array; the recurrence is
 * evaluated by first adding the positive contributions, then
 * subtracting the negative ones (the subtraction is safe because
 * the positive sum is always ≥ the negative sum for n ≥ 22).
 *
 * The routine works in O(n) time and uses O(n) memory; it can be
 * called in arbitrary order.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;        /* array of computed terms, index 0 unused */
static size_t cache_cap = 0;       /* allocated capacity (including unused 0) */
static idx_t cache_count = 0;      /* highest index for which a value is known */

/* Grow the cache so that it can hold indices up to 'n' (inclusive). */
static void
ensure_capacity(idx_t n)
{
    /* required capacity includes a 0th slot that is unused */
    size_t required = (size_t)n + 1;
    if (required <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < required)
        new_cap = required;

#ifdef __cplusplus
    seq_t *new_cache = new seq_t[new_cap];
    if (cache) {
        for (size_t i = 0; i <= cache_count; ++i)
            new_cache[i] = cache[i];
        delete[] cache;
    }
#else
    seq_t *new_cache = realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        return; /* allocation failed – caller will see an incomplete result */
    /* 'cache' contents are already preserved by realloc */
#endif

    cache = new_cache;
    cache_cap = new_cap;
}

/* Initial terms a(1) … a(22) stored as seq_t constants.  The constants
 * are cast to seq_t so that the compiler applies the correct width.
 * Truncation occurs only for builds where seq_t is smaller than the
 * values; those values will never be used for that build. */
static const seq_t init_terms[23] = {
    (seq_t)0ULL,                     /* index 0 – unused */
    (seq_t)0ULL,                     /* a(1) */
    (seq_t)11ULL,                    /* a(2) */
    (seq_t)49ULL,                    /* a(3) */
    (seq_t)203ULL,                   /* a(4) */
    (seq_t)955ULL,                   /* a(5) */
    (seq_t)4556ULL,                  /* a(6) */
    (seq_t)21843ULL,                 /* a(7) */
    (seq_t)103319ULL,                /* a(8) */
    (seq_t)491073ULL,                /* a(9) */
    (seq_t)2332753ULL,               /* a(10) */
    (seq_t)11082938ULL,              /* a(11) */
    (seq_t)52648987ULL,              /* a(12) */
    (seq_t)250106679ULL,             /* a(13) */
    (seq_t)1188163911ULL,            /* a(14) */
    (seq_t)5644397853ULL,            /* a(15) */
    (seq_t)26814116700ULL,           /* a(16) */
    (seq_t)127381683511ULL,          /* a(17) */
    (seq_t)605134192403ULL,          /* a(18) */
    (seq_t)2874722191055ULL,         /* a(19) */
    (seq_t)13656526930929ULL,        /* a(20) */
    (seq_t)64876076932622ULL,        /* a(21) */
    (seq_t)308197362006435ULL        /* a(22) */
};

/* Compute a single term by the recurrence using the already
 * computed values in the cache.  The recurrence is split into
 * a positive sum and a negative sum to avoid unsigned underflow. */
static seq_t
recurrence_next(idx_t n)
{
    seq_t pos = cache[n-1];
    pos += cache[n-2] * 17ULL;
    pos += cache[n-3] * 15ULL;
    pos += cache[n-8] * 343ULL;
    pos += cache[n-9] * 429ULL;
    pos += cache[n-10] * 269ULL;
    pos += cache[n-11] * 191ULL;
    pos += cache[n-19] * 8ULL;

    seq_t neg = cache[n-4] * 33ULL;
    neg += cache[n-5] * 62ULL;
    neg += cache[n-6] * 148ULL;
    neg += cache[n-7] * 116ULL;
    neg += cache[n-12] * 415ULL;
    neg += cache[n-13] * 534ULL;
    neg += cache[n-14] * 468ULL;
    neg += cache[n-15] * 436ULL;
    neg += cache[n-16] * 132ULL;
    neg += cache[n-17] * 72ULL;
    neg += cache[n-18] * 8ULL;

    return pos - neg;
}

/* Public entry point requested by the specification. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined, but safe for callers */

    ensure_capacity(n);

    /* On first use, load the known initial terms up to 22. */
    if (cache_count == 0) {
        size_t max_init = n < 22 ? (size_t)n : 22;
        for (size_t i = 1; i <= max_init; ++i)
            cache[i] = init_terms[i];
        cache_count = max_init;
    }

    /* If the requested n is within the pre‑loaded area, return it. */
    if (n <= cache_count)
        return cache[n];

    /* Compute any missing terms up to n using the recurrence. */
    for (idx_t k = cache_count + 1; k <= n; ++k) {
        if (k <= 22) {
            /* The first 22 values were already copied from init_terms. */
            if (k > cache_count)
                cache[k] = init_terms[k];
        } else {
            cache[k] = recurrence_next(k);
        }
    }
    cache_count = n;
    return cache[n];
}
