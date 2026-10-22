/*
 * A337615  T(n, k) = binomial(n, k)*sf(n-k)*sf(k) where sf is the
 *          subfactorial (A000166). Triangle read by rows, for 0 <= k <= n.
 * https://oeis.org/A337615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(r,k) * sf(r-k) * sf(k) where n = r(r+1)/2 + k, 0
 *            <= k <= r. Here sf(m) is the subfactorial (A000166)
 * Range:     u8 n <= 20, u16 n <= 44, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include "../utils/binom.h"

static seq_t *sf_cache = NULL;
static idx_t sf_len = 0;

static void ensure_sf(idx_t n)
{
    if (sf_len <= n)
    {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)realloc(sf_cache, new_len * sizeof(seq_t));
        if (!new_cache)
            return; /* allocation failure – let the caller handle it */

        /* initialize known values */
        if (new_len > 0)
            new_cache[0] = (seq_t)1;
        if (new_len > 1)
            new_cache[1] = (seq_t)0;

        for (idx_t i = 2; i < new_len; ++i)
        {
            /* sf[i] = (i-1)*(sf[i-1] + sf[i-2]) */
            new_cache[i] = (seq_t)(i - 1) * (new_cache[i - 1] + new_cache[i - 2]);
        }

        sf_cache = new_cache;
        sf_len = new_len;
    }
}

static inline seq_t subfact(idx_t n)
{
    ensure_sf(n);
    return sf_cache[n];
}

seq_t a(idx_t n)
{
    /* Find row r so that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = (idx_t)((sqrt((long double)8 * n + 1) - 1) / 2);
    /* Correct possible off‑by‑one due to floating point rounding */
    while ((unsigned __int128)r * (r + 1) / 2 > n)
        --r;
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Column offset within row r */
    idx_t k = n - (idx_t)((unsigned __int128)r * (r + 1) / 2);

    seq_t bin = binom(r, k);
    seq_t sf_k = subfact(k);
    seq_t sf_rk = subfact(r - k);

    return bin * sf_k * sf_rk;
}
