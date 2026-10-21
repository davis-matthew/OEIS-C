/*
 * A295930  Largest number with exactly n representations as a sum of eight
 *          nonnegative squares.
 * https://oeis.org/A295930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 9, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS sequence A295930: largest number with exactly
 * n unordered representations as a sum of eight nonnegative squares.
 * The number of representations is counted up to permutation of the eight
 * terms (zeros are allowed to pad to eight elements).  The function uses a
 * dynamic‑programming partition algorithm that counts unordered multisets
 * of square values.  The implementation is valid C11 and C++17.  The
 * same code is used for all builds; seq_t is the integer type supplied
 * by the framework (e.g. uint32_t, unsigned __int128, or an arbitrary-
 * precision class).  For lazy initialization the DP is performed on the
 * first call; subsequent calls use the pre‑computed table. */

#include <stddef.h>
#include <stdlib.h>

/* Integer square root: largest r such that r*r <= n. */
static inline idx_t _floor_isqrt_u64(idx_t n) {
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= n) ++r;
    return r;
}

/* Global state for the pre‑computed table. */
static seq_t *max_for_count = NULL;
static idx_t max_count_val = 0;

/* One‑time initialization: compute representation counts up to a bound
 * that guarantees all required n (up to several ten thousands). */
static void init(void)
{
    const idx_t MAX_M = 20000;          /* adjustable upper bound */
    const size_t ROWS = 9;              /* 0 … 8 squares */
    const size_t COLS = (size_t)MAX_M + 1UL;
    const size_t DP_SIZE = ROWS * COLS;

    seq_t *dp = (seq_t *)calloc(DP_SIZE, sizeof(seq_t));
    if (!dp) return;                   /* unlikely – abort on allocation failure */

    /* Base case: one way to represent 0 with 0 squares. */
    dp[0 * COLS + 0] = 1;

    /* Process each square value once (coin change DP). */
    for (idx_t a = 0; a <= _floor_isqrt_u64(MAX_M); ++a) {
        idx_t v = a * a;
        for (size_t part = 1; part <= 8; ++part) {
            for (size_t sum = (size_t)v; sum <= MAX_M; ++sum) {
                dp[part * COLS + sum] += dp[(part - 1) * COLS + (sum - v)];
            }
        }
    }

    /* Find maximum count seen to size the mapping array. */
    idx_t local_max = 0;
    for (size_t m = 0; m <= MAX_M; ++m) {
        idx_t cnt = (idx_t)dp[8 * COLS + m];
        if (cnt > local_max) local_max = cnt;
    }

    /* Allocate array mapping count → largest m with that count. */
    seq_t *cache = (seq_t *)calloc((size_t)local_max + 1U, sizeof(seq_t));
    if (!cache) {
        free(dp);
        return;
    }

    for (size_t m = 0; m <= MAX_M; ++m) {
        idx_t cnt = (idx_t)dp[8 * COLS + m];
        if (cnt > 0) {
            cache[cnt] = (seq_t)m;      /* later m overwrites earlier → largest */
        }
    }

    free(dp);
    max_for_count = cache;
    max_count_val = local_max;
}

/* Public API for OEIS test harness. */
seq_t a(idx_t n)
{
    static int initialized = 0;
    if (!initialized) {
        init();
        initialized = 1;
    }

    if (n == 0 || n > max_count_val) return 0;
    return max_for_count[n];
}
