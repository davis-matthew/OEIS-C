/*
 * A274667  Diagonal of the rational function 1/(1 - x - y - x y - x z - y z +
 *          x y z).
 * https://oeis.org/A274667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the 4‑term recurrence * * 2*n^2*(469*n^2 - 2106*n
 *            + 2229)*a(n) = * (11725*n^4 - 64375*n^3 + 111011*n^2 - 68153*n +
 *            13344)*a(n-1) * + (46431*n^4 - 301356*n^3 + 678782*n^2 -
 *            620403*n + 186048)*a(n-2) * + (37989*n^4 - 284553*n^3 +
 *            757682*n^2 - 829732*n + 299712)*a(n-3) * - 2*(n-3)^2*(...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <cstring>
#endif

#ifdef __SIZEOF_INT128__
typedef unsigned __int128 u128;
#endif

/* Cache for computed values; grows up to requested index.
 * For arbitrary‑precision seq_t objects the allocation is done via
 * new[] (C++) or malloc (C).  Elements are copied when the array
 * is enlarged.  The initial four terms are filled on first use. */
static seq_t *cache = NULL;
static size_t cache_alloc = 0;   /* number of allocated slots (0 means no cache) */
static idx_t  cache_max  = (idx_t)-1;  /* highest index computed (uninitialised) */

/* Ensure the cache is allocated with at least 'need+1' slots
 * and that indices 0..3 contain the initial values. */
static void ensure_cache_alloc(size_t need)
{
    if (cache_alloc > need) return;          /* already big enough */

    size_t new_sz = need + 1;
    seq_t *new_cache = NULL;

#ifdef __cplusplus
    /* C++: use new[] for correctness with non‑POD types */
    new_cache = new seq_t[new_sz];
    if (cache) {
        for (size_t i = 0; i <= (size_t)cache_max; ++i)
            new_cache[i] = cache[i];
        delete[] cache;
    }
#else
    /* C: use malloc; seq_t is assumed to be POD in this build */
    new_cache = (seq_t *)malloc(new_sz * sizeof(seq_t));
    if (!new_cache) return;        /* out of memory – unlikely for the test range */
    if (cache) {
        memcpy(new_cache, cache, (size_t)(cache_max + 1) * sizeof(seq_t));
        free(cache);
    }
#endif

    cache      = new_cache;
    cache_alloc = new_sz;

    /* initialise the first four terms on the very first allocation */
    if (cache_max == (idx_t)-1) {
        cache[0] = 1; cache[1] = 3; cache[2] = 31; cache[3] = 339;
        cache_max = 3;
    }
}

/* Compute a(n) using the recurrence.  This function assumes that
 * the cache is large enough to hold indices up to n.  It also
 * guarantees that indices 0..3 are already initialised. */
static void compute_up_to(idx_t n)
{
    if (n <= cache_max) return;          /* already computed */

    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t cache_i_1 = cache[i-1];
        seq_t cache_i_2 = cache[i-2];
        seq_t cache_i_3 = cache[i-3];
        seq_t cache_i_4 = cache[i-4];

        seq_t ns = (seq_t)i;
        seq_t n2 = ns * ns;
        seq_t n3 = n2 * ns;
        seq_t n4 = n3 * ns;

#ifdef __SIZEOF_INT128__
        /* Use 128‑bit intermediate when seq_t is 8 bytes or smaller. */
        if (sizeof(seq_t) <= 8) {
            u128 nn = (u128)i;
            u128 n2U = nn * nn;
            u128 n3U = n2U * nn;
            u128 n4U = n3U * nn;

            u128 term1 = (u128)11725 * n4U
                       - (u128)64375 * n3U
                       + (u128)111011 * n2U
                       - (u128)68153 * nn
                       + 13344;

            u128 term2 = (u128)46431 * n4U
                       - (u128)301356 * n3U
                       + (u128)678782 * n2U
                       - (u128)620403 * nn
                       + 186048;

            u128 term3 = (u128)37989 * n4U
                       - (u128)284553 * n3U
                       + (u128)757682 * n2U
                       - (u128)829732 * nn
                       + 299712;

            u128 inner4 = (u128)469 * n2U
                        - (u128)1168 * nn
                        + 592;
            u128 term4 = (u128)2 * (nn - 3) * (nn - 3) * inner4;

            u128 denom = (u128)2 * n2U * ((u128)469 * n2U
                                            - (u128)2106 * nn + 2229);

            u128 num = term1 * (u128)cache_i_1
                     + term2 * (u128)cache_i_2
                     + term3 * (u128)cache_i_3
                     - term4 * (u128)cache_i_4;

            cache[i] = (seq_t)(num / denom);
            continue;
        }
#endif
        /* generic (big‑integer) case */
        seq_t term1 = (seq_t)11725 * n4
                    - (seq_t)64375 * n3
                    + (seq_t)111011 * n2
                    - (seq_t)68153 * ns
                    + 13344;

        seq_t term2 = (seq_t)46431 * n4
                    - (seq_t)301356 * n3
                    + (seq_t)678782 * n2
                    - (seq_t)620403 * ns
                    + 186048;

        seq_t term3 = (seq_t)37989 * n4
                    - (seq_t)284553 * n3
                    + (seq_t)757682 * n2
                    - (seq_t)829732 * ns
                    + 299712;

        seq_t inner4 = (seq_t)469 * n2
                    - (seq_t)1168 * ns
                    + 592;
        seq_t term4 = (seq_t)2 * (ns - 3) * (ns - 3) * inner4;

        seq_t denom = (seq_t)2 * n2 * ((seq_t)469 * n2
                                        - (seq_t)2106 * ns + 2229);

        seq_t num = term1 * cache_i_1
                  + term2 * cache_i_2
                  + term3 * cache_i_3
                  - term4 * cache_i_4;

        cache[i] = num / denom;
    }
    cache_max = n;
}

/* Public sequence generator */
seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 3;
    if (n == 2) return 31;
    if (n == 3) return 339;

    /* Ensure cache is large enough and compute up to n */
    ensure_cache_alloc((size_t)n);
    compute_up_to(n);
    return cache[n];
}
