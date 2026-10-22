/*
 * A344807  Numbers that are the sum of six squares in three or more ways.
 * https://oeis.org/A344807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 216, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers that are the sum of six squares in three or more ways.
   For an index n ≥ 1, a(n) returns the n‑th such number.
   The implementation keeps a cache of all qualifying numbers up
   to a dynamically chosen limit, extending it as required. */

#include <stdlib.h>
#include <stdbool.h>

/* ---- internal cache structure ---------------------------------------- */
static struct {
    seq_t  *seq;     /* array of qualifying numbers */
    size_t seq_len;  /* number of elements in seq   */
    idx_t  limit;    /* largest number that has been examined */
} _a_cache = {NULL, 0, 1000};

/* ---- recompute the cache for all numbers ≤ limit -------------------- */
static void _recompute_cache(idx_t limit)
{
    if (_a_cache.seq) {
        free(_a_cache.seq);
        _a_cache.seq = NULL;
        _a_cache.seq_len = 0;
    }

    /* Allocate DP table: 7 rows (0..6 squares) × (limit+1) columns. */
    size_t rows = 7;
    size_t cols = (size_t)limit + 1;
    size_t dp_size = rows * cols;
    uint8_t *dp = (uint8_t *)calloc(dp_size, sizeof(uint8_t));
    if (!dp)
        return;  /* allocation failure – leave cache empty */

    /* Base case: 0 can be represented with 0 squares in exactly one way. */
    dp[0] = 1;  /* dp[0][0] */

    /* Compute number of squares: find largest integer with s*s ≤ limit. */
    idx_t max_root = 0;
    while ((max_root + 1) * (max_root + 1) <= limit)
        ++max_root;

    /* DP over all square values s^2. */
    for (idx_t val = 1; val <= max_root; ++val) {
        idx_t sq = val * val;
        for (int k = 1; k <= 6; ++k) {
            size_t offset_k   = (size_t)k * cols;          /* dp[k][0] index */
            size_t offset_km1 = (size_t)(k - 1) * cols;    /* dp[k-1][0] index */
            for (idx_t n = sq; n <= limit; ++n) {
                uint8_t prev = dp[offset_km1 + (n - sq)];
                if (!prev)
                    continue;
                uint8_t curr = dp[offset_k + n];
                uint8_t add  = prev;
                uint8_t newv = curr + add;
                if (newv > 3)    /* we only care up to 3 */
                    newv = 3;
                dp[offset_k + n] = newv;
            }
        }
    }

    /* Count qualifying numbers to allocate the result array. */
    size_t count = 0;
    size_t offset6 = 6 * cols;
    for (size_t n = 0; n < cols; ++n) {
        if (dp[offset6 + n] >= 3)
            ++count;
    }

    /* Build the sequence array. */
    seq_t *seq = (seq_t *)malloc(count * sizeof(seq_t));
    if (!seq) {
        free(dp);
        return;  /* allocation failure */
    }

    size_t pos = 0;
    for (size_t n = 0; n < cols; ++n) {
        if (dp[offset6 + n] >= 3)
            seq[pos++] = (seq_t)n;
    }

    free(dp);

    _a_cache.seq     = seq;
    _a_cache.seq_len = count;
    _a_cache.limit   = limit;
}

/* ---- ensure that the cache contains at least n terms. ---------------- */
static void _ensure_cache(idx_t n)
{
    if (!_a_cache.seq)
        _recompute_cache(_a_cache.limit);

    while (_a_cache.seq_len < (size_t)n) {
        /* Double the limit to reduce recomputations. */
        _a_cache.limit = (_a_cache.limit ? _a_cache.limit * 2 : 1000);
        _recompute_cache(_a_cache.limit);
    }
}

/* ---- exported sequence function --------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for 0, safety */
    _ensure_cache(n);
    return _a_cache.seq[(size_t)(n - 1)];
}
