/*
 * A345522  Numbers that are the sum of seven cubes in four or more ways.
 * https://oeis.org/A345522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A345522: numbers that are the sum of seven cubes in four or more ways.
   Algorithm: dynamic programming with unlimited supply of each cube
   (order of cubes ignored). For each cube value v (a³) we update the
   number of ways to choose j cubes for all j=1..7. Counts are capped at 4.
   After processing all cubes up to a chosen limit, all numbers with
   at least four representations are output in increasing order.
   The precomputation is performed once on the first call to a(). */

#include <stdlib.h>
#include <stdbool.h>

static const idx_t LIMIT_MAX = 1000000U;       /* upper bound for sums */
static seq_t *seq_cache = NULL;                /* array of sequence values */
static idx_t  seq_len   = 0;                   /* number of values stored   */
static bool   seq_ready = false;              /* whether precomputation finished */

static void compute_sequence(void)
{
    const idx_t maxSum = LIMIT_MAX;
    const size_t rows = 8;                     /* 0..7 cubes chosen      */
    const size_t width = (size_t)(maxSum + 1);
    const size_t dp_size = rows * width;

    /* Allocate DP table: dp[j][s] stored as a flat array */
    uint8_t *dp = (uint8_t *)malloc(dp_size);
    if (!dp) { abort(); }
    for (size_t i = 0; i < dp_size; ++i) dp[i] = 0;
    dp[0] = 1;                                 /* dp[0][0] = 1 */

    /* Process each cube value v = a³ <= maxSum */
    for (uint64_t a = 1; ; ++a) {
        uint64_t v64 = a * a * a;
        if (v64 > maxSum) break;
        int v = (int)v64;                      /* v <= maxSum <= 1e6 */

        for (int j = 1; j <= 7; ++j) {
            size_t jmul   = (size_t)j * width;
            size_t prevmul = (size_t)(j - 1) * width;
            for (int s = v; s <= maxSum; ++s) {
                uint8_t prev = dp[prevmul + (s - v)];
                uint8_t cur  = dp[jmul + s];
                int sum = cur + prev;
                if (sum > 4) sum = 4;
                dp[jmul + s] = (uint8_t)sum;
            }
        }
    }

    /* Allocate sequence buffer */
    size_t cap = 20000;                        /* initial capacity   */
    seq_cache = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!seq_cache) { abort(); }

    /* Collect numbers with at least four representations */
    for (int s = 1; s <= maxSum; ++s) {
        if (dp[7 * width + s] >= 4) {
            if (seq_len == cap) {
                cap <<= 1;                      /* double capacity   */
                seq_cache = (seq_t *)realloc(seq_cache, cap * sizeof(seq_t));
                if (!seq_cache) { abort(); }
            }
            seq_cache[seq_len++] = (seq_t)s;
        }
    }

    free(dp);
    seq_ready = true;
}

seq_t a(idx_t n)
{
    if (!seq_ready) compute_sequence();
    if (n == 0) return 0;
    if (n - 1 < seq_len) return seq_cache[n - 1];
    return 0;   /* should not happen for required n */
}
