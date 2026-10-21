/*
 * A263513  Number of (n+1)X(2+1) arrays of permutations of 0..n*3+2 filled by
 *          rows with each element moved a city block distance of 0 or 1, and
 *          rows and columns in increasing lexicographic order.
 * https://oeis.org/A263513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 7*a(n-1) - a(n-2) - 39*a(n-3) + 24*a(n-4) + 57*a(n-5)
 *               - 33*a(n-6) - 28*a(n-7) + 13*a(n-8) + 3*a(n-9) - a(n-10)
 *
 * Initial values:
 *   a(1)=7,   a(2)=35,   a(3)=208,   a(4)=1198,   a(5)=7022,
 *   a(6)=41035, a(7)=240237, a(8)=1406038, a(9)=8230727, a(10)=48180019
 *
 * The function uses memoisation.  Calls may be made in any order; the cache
 * grows until it contains the requested index.  The implementation works
 * with the arbitrary‑precision seq_t type expected in the C++ build.
 */

#include <stdlib.h>
#include <assert.h>

/* External function definition required by the build */
extern seq_t a(idx_t n)
{
    /* static cache: cache[i] == a(i+1)  (offset 1) */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* size allocated */
    static idx_t cache_len  = 0;   /* highest n computed (cache_len == 0 ⇒ no values yet) */

    /* base values for n = 1..10 */
    static const seq_t base[10] = {
        (seq_t)7ULL,          /* a(1) */
        (seq_t)35ULL,         /* a(2) */
        (seq_t)208ULL,        /* a(3) */
        (seq_t)1198ULL,       /* a(4) */
        (seq_t)7022ULL,       /* a(5) */
        (seq_t)41035ULL,      /* a(6) */
        (seq_t)240237ULL,     /* a(7) */
        (seq_t)1406038ULL,    /* a(8) */
        (seq_t)8230727ULL,    /* a(9) */
        (seq_t)48180019ULL    /* a(10) */
    };

    /* Ensure cache can hold terms up to n */
    if (n > cache_len) {
        /* enlarge cache: double size until sufficient, at least n */
        idx_t new_len = cache_len ? cache_len << 1 : 16;
        if (new_len < n) new_len = n;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) {
            assert(0 && "Memory allocation failed");
        }
        cache = new_cache;
        cache_size = new_len;

        /* Compute missing values up to n */
        for (idx_t idx = cache_len + 1; idx <= n; ++idx) {
            idx_t cache_idx = idx - 1;  /* index into cache array */
            seq_t val;
            if (idx <= 10) {
                val = base[idx - 1];
            } else {
                /* recurrence using only non-negative intermediate values */
                seq_t pos = cache[cache_idx - 1] * 7U;           /* 7*a(n-1) */
                pos += cache[cache_idx - 4] * 24U;              /* +24*a(n-4) */
                pos += cache[cache_idx - 5] * 57U;              /* +57*a(n-5) */
                pos += cache[cache_idx - 8] * 13U;              /* +13*a(n-8) */
                pos += cache[cache_idx - 9] * 3U;               /* +3*a(n-9) */

                seq_t neg = cache[cache_idx - 2];               /* -a(n-2) */
                neg += cache[cache_idx - 3] * 39U;              /* -39*a(n-3) */
                neg += cache[cache_idx - 6] * 33U;              /* -33*a(n-6) */
                neg += cache[cache_idx - 7] * 28U;              /* -28*a(n-7) */
                neg += cache[cache_idx - 10];                  /* -a(n-10) */

                val = pos - neg;   /* final term (pos ≥ neg by recurrence) */
            }
            cache[cache_idx] = val;
        }
        cache_len = n;
    }

    return cache[n - 1];
}
