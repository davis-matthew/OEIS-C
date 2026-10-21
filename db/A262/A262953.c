/*
 * A262953  Expansion of Product_{k>=1} (1 + x^(2*k-1)) * (1 + x^(3*k-2)).
 * https://oeis.org/A262953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts of * odd size or of
 *            size ≡1 (mod 3), with multiplicity at most * 2 for parts ≡1 (mod
 *            6) (i.e. odd and 1 mod 3) and at most 1 * for all other allowed
 *            parts. The generating function is * * G(x) = ∏_{k≥1} (1 +
 *            x^{2k-1}) · ∏_{k≥1} (1 + x^{3k-2}) * * The imp...
 * Range:     u8 n <= 32, u16 n <= 90, u32 n <= 279, u64 n <= 939,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *CACHE = NULL;     /* array holding a[0..max_n]        */
static size_t CACHE_SIZE = 0;   /* current capacity of CACHE         */

/* Rebuild the cache up to at least n (inclusive).  The algorithm
 * is the ordinary bounded coin‑change DP: each weight w contributes
 * a factor (1 + x^w) if w is odd or 1 mod 3, and (1 + 2x^w + x^{2w})
 * if w is ≡1 (mod 6).  In practice this is handled by applying the
 * single‑copy update once or twice per weight.  The cache is
 * rebuilt from scratch each time it must grow.                */
static void build_cache(idx_t n)
{
    /* allocate new array of size n+1, initialized to 0 */
    seq_t *new_cache = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!new_cache) abort();                /* allocation failure */

    for (idx_t i = 0; i <= n; ++i)
        new_cache[i] = 0;
    new_cache[0] = 1;                       /* empty partition */

    /* process all weights w from 1 to n */
    for (idx_t w = 1; w <= n; ++w)
    {
        int e = ((w & 1u) ? 1 : 0) + ((w % 3u) == 1u ? 1 : 0);  /* multiplicity */
        if (e == 0) continue;

        for (int rep = 0; rep < e; ++rep)          /* repeat 1 or 2 times */
        {
            for (idx_t s = n; s >= w; --s)          /* downward to avoid reuse */
                new_cache[s] += new_cache[s - w];
        }
    }

    /* swap old cache */
    if (CACHE) free(CACHE);
    CACHE      = new_cache;
    CACHE_SIZE = (size_t)(n + 1);
}

/*  Main function requested by the build system  */
seq_t a(idx_t n)
{
    if (!CACHE || (size_t)n + 1 > CACHE_SIZE)
        build_cache(n);
    return CACHE[n];
}
