/*
 * A386793  Coefficients in level 23 Ramanujan-Sato series for 1/Pi.
 * https://oeis.org/A386793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   (n + 1)^3*a(n + 1) = (2n + 1)(7n^2 + 7n + 4)a(n) - n(57n^2 +
 *            31)a(n - 1) + (2n - 1)(53n^2 - 53n + 42)a(n - 2) - 6(n -
 *            1)(15n^2 - 30n + 19)a(n - 3) + 8(n - 1)(n - 2)(2n - 3)a(n - 4) +
 *            19(n - 1)(n - 2)(n - 3)a(n - 5) with a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#if defined(__cplusplus)
#include <cstddef>
#include <cstdlib>
#else
#include <stddef.h>
#include <stdlib.h>
#endif

static seq_t *cache = NULL;       /* memoised sequence values           */
static size_t  cache_size = 0;    /* allocated size of cache            */
static size_t  computed = 0;      /* highest index that has been computed */

#if defined(__cplusplus)
#define ALLOC_SEQ_T(sz) new seq_t[(sz)]
#define FREE_SEQ_T(p)   delete [] (p)
#else
#define ALLOC_SEQ_T(sz) ((seq_t*)calloc((sz), sizeof(seq_t)))
#define FREE_SEQ_T(p)   free(p)
#endif

/* Ensure that cache[0..n] exist and fill missing terms.          */
static void ensure_computed(idx_t n)
{
    /* Allocate or grow the cache if necessary. */
    if (!cache) {
        cache = ALLOC_SEQ_T((size_t)n + 1);
        cache_size = (size_t)n + 1;
    } else if (cache_size <= (size_t)n + 1) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = ALLOC_SEQ_T(new_size);
        /* copy existing values */
        for (size_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];
        FREE_SEQ_T(cache);
        cache = new_cache;
        cache_size = new_size;
    }

    /* Initialise a(0) if not yet done. */
    if (computed == 0 && cache[0] == 0) {
        cache[0] = (seq_t)1;
        computed = 0;
    }

    /* Compute terms that are still missing. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        idx_t N = i - 1;  /* N corresponds to n in the recurrence    */
        /* Use 128‑bit signed arithmetic for intermediates. */
        __int128 rhs = 0;
        __int128 term0 = (__int128)(2 * (idx_t)N + 1)
                     * ((__int128)7 * (idx_t)N * (idx_t)N
                        + (__int128)7 * (idx_t)N + 4)
                     * (__int128)cache[N];
        rhs += term0;

        if (N >= 1) {
            __int128 term1 = (__int128)(idx_t)N
                       * ((__int128)57 * (idx_t)N * (idx_t)N
                          + 31)
                       * (__int128)cache[N - 1];
            rhs -= term1;
        }

        if (N >= 2) {
            __int128 term2 = (__int128)(2 * (idx_t)N - 1)
                       * ((__int128)53 * (idx_t)N * (idx_t)N
                          - (__int128)53 * (idx_t)N + 42)
                       * (__int128)cache[N - 2];
            rhs += term2;
        }

        if (N >= 3) {
            __int128 term3 = (__int128)6 * (__int128)(N - 1)
                       * ((__int128)15 * (idx_t)N * (idx_t)N
                          - (__int128)30 * (idx_t)N + 19)
                       * (__int128)cache[N - 3];
            rhs -= term3;
        }

        if (N >= 4) {
            __int128 term4 = (__int128)8 * (__int128)(N - 1)
                       * (__int128)(N - 2)
                       * ((__int128)2 * (idx_t)N - 3)
                       * (__int128)cache[N - 4];
            rhs += term4;
        }

        if (N >= 5) {
            __int128 term5 = (__int128)19 * (__int128)(N - 1)
                       * (__int128)(N - 2)
                       * (__int128)(N - 3)
                       * (__int128)cache[N - 5];
            rhs += term5;
        }

        /* Divide by (i)^3; the division is exact. */
        __int128 denom = (__int128)i * (__int128)i * (__int128)i;
        seq_t value = (seq_t)(rhs / denom);
        cache[i] = value;
    }
    computed = (size_t)n;
}

/* Public interface requested by the build. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;      /* a(0) = 1 */
    ensure_computed(n);
    return cache[n];
}
