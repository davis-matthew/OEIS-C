/*
 * A310389  Coordination sequence Gal.3.46.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -2*a(n-1) - 2*a(n-2) + 3*a(n-4) + 4*a(n-5) + 3*a(n-6) -
 *            2*a(n-8) - 2*a(n-9) - a(n-10) for n > 11 Initial values: a(0)=1,
 *            a(1)=4, a(2)=10, a(3)=14, a(4)=18, a(5)=28, a(6)=26, a(7)=34,
 *            a(8)=42, a(9)=40, a(10)=50, a(11)=56. The function memorises the
 *            sequence in a static array so that it can...
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static cache for the sequence values */
static seq_t *cache = NULL;          /* pointer to allocated array */
/* number of elements that have been computed and stored in cache */
static size_t cache_valid_len = 0;
/* number of elements that the array was allocated for */
static size_t cache_alloc_len = 0;

/* minimal integer constants for the initial 12 values */
static const uint64_t init_int[12] = {
    1ULL, 4ULL, 10ULL, 14ULL, 18ULL, 28ULL,
    26ULL, 34ULL, 42ULL, 40ULL, 50ULL, 56ULL
};

/* ensure that the cache has at least (n+1) allocated cells */
static void ensure_cache_size(idx_t n)
{
    size_t required = (size_t)n + 1;
    if (required <= cache_alloc_len) return;
    seq_t *tmp = (seq_t*)realloc(cache, required * sizeof(seq_t));
    if (!tmp) abort();                /* allocation failed */
    cache = tmp;
    cache_alloc_len = required;
}

/* return the nth term of A310389 */
seq_t a(idx_t n)
{
    /* initialise cache lazily */
    if (cache_valid_len == 0) {
        size_t init_len = 12;
        cache = (seq_t*)calloc(init_len, sizeof(seq_t));
        if (!cache) abort();
        cache_alloc_len = init_len;
        for (idx_t i = 0; i < 12; ++i)
            cache[i] = (seq_t)init_int[i];
        cache_valid_len = 12;
    }

    /* if already computed, return it */
    if ((size_t)n < cache_valid_len)
        return cache[n];

    /* make sure array is large enough */
    ensure_cache_size(n);

    /* compute all missing values up to n using the recurrence */
    for (idx_t i = cache_valid_len; i <= n; ++i) {
        seq_t val = -((seq_t)2)*cache[i-1]
                    - ((seq_t)2)*cache[i-2]
                    + ((seq_t)3)*cache[i-4]
                    + ((seq_t)4)*cache[i-5]
                    + ((seq_t)3)*cache[i-6]
                    - ((seq_t)2)*cache[i-8]
                    - ((seq_t)2)*cache[i-9]
                    - cache[i-10];
        cache[i] = val;
    }

    cache_valid_len = (size_t)n + 1;
    return cache[n];
}
