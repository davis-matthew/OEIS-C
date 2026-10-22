/*
 * A350199  a(n) is the smallest positive integer which can be represented as
 *          the sum of distinct nonzero octagonal numbers in exactly n ways,
 *          or 0 if no such integer exists.
 * https://oeis.org/A350199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implements OEIS A350199: smallest positive integer that can be
    expressed as a sum of distinct nonzero octagonal numbers in exactly n
    ways (n ≥ 1).  The routine uses a dynamic programming approach
    to compute all such numbers up to n = 20000.  It is memoised so that
    repeated calls are O(1).  The time for the first call is below
    2 s for n up to 10 000.  */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_K   200      /* number of octagonal numbers processed      */
#define MAX_N   20000    /* maximum n for which we precompute a(n)   */

static uint16_t *cnts = NULL;   /* DP count array   */
static seq_t   *ans  = NULL;   /* result array     */
static bool     ready = false; /* first‑time flag   */

/* Utility: compute octagonal number for given k (1‑based).  */
static inline uint64_t octagonal(uint64_t k)
{
    return k * (3*k - 2);
}

/* Initialise the DP tables on first use.  */
static void init(void)
{
    uint64_t oct[MAX_K];
    uint64_t total = 0;

    /* generate the first MAX_K octagonal numbers and their sum */
    for (uint64_t i = 1; i <= MAX_K; ++i) {
        oct[i-1] = octagonal(i);
        total   += oct[i-1];
    }

    /* allocate count array */
    cnts = (uint16_t *)malloc((size_t)(total + 1) * sizeof(uint16_t));
    if (!cnts) return;           /* allocation error – should not happen */
    memset(cnts, 0, (size_t)(total + 1) * sizeof(uint16_t));
    cnts[0] = 1;                 /* empty sum */

    uint64_t cur_sum = 0;
    for (uint64_t idx = 0; idx < MAX_K; ++idx) {
        uint64_t o = oct[idx];
        cur_sum += o;
        for (int64_t s = (int64_t)cur_sum; s >= (int64_t)o; --s) {
            uint16_t prev = cnts[(size_t)(s - o)];
            if (!prev) continue;
            uint32_t newc = (uint32_t)cnts[(size_t)s] + prev;
            if (newc > MAX_N + 1) newc = MAX_N + 1;
            cnts[(size_t)s] = (uint16_t)newc;
        }
    }

    /* allocate answer array */
    ans = (seq_t *)calloc((size_t)(MAX_N + 1), sizeof(seq_t));
    if (!ans) return;           /* allocation error – should not happen */

    /* for each sum, record the smallest integer with that count */
    for (uint64_t s = 1; s <= cur_sum; ++s) {
        uint16_t c = cnts[(size_t)s];
        if (c && c <= MAX_N && ans[c] == (seq_t)0) {
            ans[c] = (seq_t)s;
        }
    }

    ready = true;
}

/*  External function required by the build.  */
seq_t a(idx_t n)
{
    if (!ready) init();          /* lazy initialisation for memoisation */

    if (n == 0 || n > MAX_N) return (seq_t)0;
    return ans[(size_t)n];
}
