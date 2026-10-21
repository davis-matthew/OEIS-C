/*
 * A253666  Triangle read by rows, T(n,k) = C(n,k)*n!/(floor(n/2)!)^2, n>=0,
 *          0<=k<=n.
 * https://oeis.org/A253666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) * where r = floor((√(8*n+1)-1)/2), * k = n -
 *            r*(r+1)/2, * T(r,k) = C(r,k) * r! / (⌊r/2⌋!)^2 * (the triangle
 *            is read row‑major)
 * Range:     u8 n <= 16, u16 n <= 44, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

/* ---------- integer square root for 128‑bit values ----------
 * returns floor(sqrt(x)) for 0 <= x <= 8*2^64.
 */
static inline uint64_t isqrt_u128(unsigned __int128 x)
{
    uint64_t lo = 0, hi = (uint64_t)1ULL << 34; /* sqrt(8*2^64) < 2^34 */
    uint64_t mid;
    while (lo <= hi) {
        mid = (lo + hi) >> 1;
        unsigned __int128 m2 = (unsigned __int128)mid * mid;
        if (m2 == x) return mid;
        if (m2 < x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

/* ---------- factorial cache (optimised for random access) ----------
 * fact_cache[i] holds i! for 0 <= i < cache_len.
 * The cache is lazily extended to cover the requested n.
 */
static seq_t *fact_cache = NULL;
static size_t cache_len   = 0;   /* number of factorials stored   */
static size_t cache_cap   = 0;   /* allocated capacity in elements */

/* Extend cache to cover factorials up to maxn (inclusive). */
static void ensure_factorial(size_t maxn)
{
    if (maxn < cache_len) return;   /* already present */

    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
    while (new_cap <= maxn) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(fact_cache, new_cap * sizeof(seq_t));
    if (tmp == NULL) abort();
    fact_cache = tmp;
    cache_cap = new_cap;

    if (cache_len == 0) {           /* first allocation: 0! = 1 */
        fact_cache[0] = (seq_t)1;
        cache_len = 1;
    }

    for (size_t i = cache_len; i <= maxn; ++i) {
        fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
    }
    cache_len = maxn + 1;
}

/* ---------- main function ----------
 * Computes the nth term as described above.
 */
seq_t a(idx_t n)
{
    /* 1. Determine row r and column k from flat index n. */
    unsigned __int128 val = (unsigned __int128)n * 8u + 1u;
    uint64_t s = isqrt_u128(val);              /* floor(sqrt(8n+1)) */
    uint64_t r = (s - 1u) / 2u;                /* row number          */
    uint64_t start = r * (r + 1u) / 2u;        /* index of first in row */
    uint64_t k = n - start;                   /* column index        */

    /* 2. Ensure factorials up to r are available. */
    ensure_factorial((size_t)r);

    /* 3. Compute factor = r! / (⌊r/2⌋!)^2 */
    uint64_t h = r >> 1;                       /* floor(r/2)          */
    seq_t denom = fact_cache[h];
    denom = denom * denom;                    /* (h!)^2              */
    seq_t factor = fact_cache[r];
    factor = factor / denom;                  /* exact division     */

    /* 4. Compute binomial coefficient C(r,k) multiplicatively. */
    seq_t binom = (seq_t)1;
    for (size_t i = 1; i <= k; ++i) {
        binom = binom * (seq_t)(r - k + i);
        binom = binom / (seq_t)i;             /* division is exact   */
    }

    /* 5. Return result. */
    return binom * factor;
}
