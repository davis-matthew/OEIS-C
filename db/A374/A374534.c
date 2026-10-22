/*
 * A374534  Expansion of (x/(4 * (1-x))) * d/dx(theta_3(x)^2).
 * https://oeis.org/A374534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/4 * sum_{i^2 + j^2 <= n, i,j in Z} (i^2 + j^2)
 * Range:     u8 n <= 24, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* integer square root: floor(sqrt(n)) implemented in O(log n) */
static idx_t isqrt_idx(idx_t n)
{
    if (n < 2) return n;
    idx_t low = 1, high = n, mid, res = 0;
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        if (mid <= n / mid) {        /* mid^2 <= n, avoid overflow */
            res = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return res;
}

/* forward declaration of cache growth routine */
static void grow_cache(size_t new_cap);

/* cached values and bookkeeping */
static seq_t *cache       = NULL;   /* a(0) ... a(max_cached) */
static size_t cache_cap   = 0;       /* allocated capacity            */
static size_t cache_size  = 0;       /* number of elements computed   */

/* ensure `cache[n]` exists (grow the cache if necessary) */
static void ensure_cache(idx_t n)
{
    if (cache_size == 0) {           /* initialise cache with a(0) */
        grow_cache(1);
        cache[0] = 0;
        cache_size = 1;
    }

    if ((size_t)n + 1 > cache_cap)
        grow_cache((size_t)n + 1);

    for (idx_t k = cache_size; k <= n; ++k) {
        idx_t r2 = 0;                      /* representations count for k */
        idx_t sqrt_k = isqrt_idx(k);

        for (idx_t i = 0; i <= sqrt_k; ++i) {
            idx_t i2 = i * i;
            idx_t j2 = k - i2;
            idx_t j  = isqrt_idx(j2);

            if (j > i) continue;           /* keep unordered pairs (i,j) */
            if (j * j != j2) continue;     /* not a perfect square */

            /* multiplicity of the unordered pair (i,j) in the full lattice */
            if (i == 0 && j == 0) {
                r2 += 1;    /* only (0,0) */
            } else if (i == 0 || j == 0) {
                r2 += 4;    /* (±i,0),(0,±i) */
            } else if (i == j) {
                r2 += 4;    /* (±i,±i) */
            } else {
                r2 += 8;    /* (±i,±j) and swapped */
            }
        }

        /* contribution of index k: k * r2 / 4 */
        unsigned __int128 prod = (unsigned __int128)k * r2;
        seq_t mult = (seq_t)(prod / 4);
        cache[k] = cache[k - 1] + mult;
    }

    cache_size = (size_t)n + 1;
}

/* grow the cache array if needed */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                           /* unrecoverable error */
    cache = tmp;
    cache_cap = new_cap;
}

/* public sequence function */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
