/*
 * A294142  Number of partitions of n into distinct odd parts that do not
 *          divide n.
 * https://oeis.org/A294142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Number of partitions of n into distinct odd parts that do not
 *            divide n
 * Range:     u8 n <= 72, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes the value by a direct DP for the
 * particular n.  Results are cached so that if the function is called
 * repeatedly with the same n no recomputation is necessary. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* The types seq_t and idx_t are provided by the build environment. */
/* For a C build seq_t is an unsigned integer type; for a C++ build it
 * may be an arbitrary-precision integer class.  The code below uses
 * only the operators + and assignment which are defined for all
 * permitted seq_t types. */

static seq_t *cache = NULL;          /* cached results for a(n)       */
static unsigned char *computed = NULL;/* flag indicating cache entry valid */
static size_t cache_cap = 0;          /* size of allocated cache       */

/* ensure that cache arrays are large enough for indices up to n   */
static void ensure_cache(size_t n)
{
    if (n < cache_cap)
        return;
    size_t new_cap = n + 1;
    /* grow by 1.5× each time to keep amortised size reasonable */
    if (new_cap < cache_cap * 3 / 2 + 16)
        new_cap = cache_cap * 3 / 2 + 16;
    if (cache_cap == 0) {
        cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        computed = (unsigned char*)malloc(new_cap * sizeof(unsigned char));
    } else {
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        computed = (unsigned char*)realloc(computed, new_cap * sizeof(unsigned char));
    }
    if (cache == NULL || computed == NULL)
        abort();  /* out of memory */
    /* initialise new part of the cache to zero valid flags   */
    memset(computed + cache_cap, 0, (new_cap - cache_cap) * sizeof(unsigned char));
    cache_cap = new_cap;
}

/* main entry point required by the platform */
seq_t a(idx_t n)
{
    size_t N = (size_t)n;              /* safe cast – idx_t is unsigned           */
    ensure_cache(N);
    if (computed[N])
        return cache[N];

    /* DP array on the stack would be too large; allocate dynamically. */
    seq_t *dp = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    if (dp == NULL)
        abort();  /* out of memory */
    dp[0] = (seq_t)1;
    /* initialize the rest to zero – malloc may already do this on many
     * systems, but be explicit for safety. */
    memset(dp + 1, 0, N * sizeof(seq_t));

    for (size_t i = 1; i <= N; i += 2) {      /* odd parts only */
        if (N % i == 0)                       /* part would divide n – skip */
            continue;
        for (size_t s = N; s >= i; --s)        /* inclusion–exclusion DP */
            dp[s] += dp[s - i];
    }

    seq_t result = dp[N];
    free(dp);

    cache[N] = result;
    computed[N] = 1;
    return result;
}
