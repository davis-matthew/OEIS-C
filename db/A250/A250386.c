/*
 * A250386  Number of length n+3 0..7 arrays with no four consecutive terms
 *          having the maximum of any two terms equal to the minimum of the
 *          remaining two terms.
 * https://oeis.org/A250386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 8, u64 n <= 20, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A250386 (offset 1):
   a(n) = number of length n+3 arrays over {0,…,7}
   with no four consecutive terms having the maximum of any two
   equal to the minimum of the remaining two terms.
   The function below computes the terms by dynamic programming over
   the last three symbols. */

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/* Precomputation flags and cache                                           */
static bool _a250386_precomp_done = false;
static idx_t _a250386_computed_n  = 0;      /* largest n cached          */
static idx_t _a250386_computed_len = 3;     /* DP length processed (cur) */

#define _A250386_CACHE_SIZE 20001
static seq_t _a250386_cache[_A250386_CACHE_SIZE];

/* --------------------------------------------------------------------- */
/* DP tables for the last three symbols (a,b,c) → count of sequences   */
static seq_t _a250386_dp_cur[512];
static seq_t _a250386_dp_nxt[512];
static seq_t *dp_cur = _a250386_dp_cur;
static seq_t *dp_nxt = _a250386_dp_nxt;

/* --------------------------------------------------------------------- */
/* Helper tables for state decomposition and validity table            */
static uint8_t _a250386_a[512];
static uint8_t _a250386_b[512];
static uint8_t _a250386_c[512];
static bool _a250386_valid[512][8];

/* --------------------------------------------------------------------- */
static void _a250386_precompute(void)
{
    /* decompose state index into a,b,c components */
    for (idx_t idx = 0; idx < 512; ++idx) {
        _a250386_a[idx] = (uint8_t)(idx >> 6);
        _a250386_b[idx] = (uint8_t)((idx >> 3) & 7);
        _a250386_c[idx] = (uint8_t)(idx & 7);
    }

    /* precompute whether adding a fourth element d keeps the condition
       satisfied for the state (a,b,c) – both directions of each partition
       must be checked. */
    for (idx_t idx = 0; idx < 512; ++idx) {
        uint8_t a = _a250386_a[idx];
        uint8_t b = _a250386_b[idx];
        uint8_t c = _a250386_c[idx];

        for (idx_t d = 0; d < 8; ++d) {
            bool bad = false;
            uint8_t max1, min2;

            /* (a,b) vs (c,d) – both directions */
            max1 = (a > b) ? a : b;
            min2 = (c < d) ? c : d;
            if (max1 == min2) bad = true;
            max1 = (c > d) ? c : d;
            min2 = (a < b) ? a : b;
            if (max1 == min2) bad = true;

            /* (a,c) vs (b,d) – both directions */
            if (!bad) {
                max1 = (a > c) ? a : c;
                min2 = (b < d) ? b : d;
                if (max1 == min2) bad = true;
                max1 = (b > d) ? b : d;
                min2 = (a < c) ? a : c;
                if (max1 == min2) bad = true;
            }

            /* (a,d) vs (b,c) – both directions */
            if (!bad) {
                max1 = (a > d) ? a : d;
                min2 = (b < c) ? b : c;
                if (max1 == min2) bad = true;
                max1 = (b > c) ? b : c;
                min2 = (a < d) ? a : d;
                if (max1 == min2) bad = true;
            }

            _a250386_valid[idx][d] = !bad;
        }
    }

    /* initialise DP for length 3: one occurrence of each triple */
    for (idx_t i = 0; i < 512; ++i) dp_cur[i] = 1;
    _a250386_precomp_done = true;
}

/* --------------------------------------------------------------------- */
static void _a250386_ensure_upto(idx_t target_n)
{
    if (!_a250386_precomp_done) _a250386_precompute();

    if (target_n <= _a250386_computed_n) return;

    /* extend DP from current length to target_n + 3 */
    for (idx_t len = _a250386_computed_len + 1; len <= target_n + 3; ++len) {
        /* reset next array */
        memset(dp_nxt, 0, sizeof(_a250386_dp_nxt));

        /* transition from current to next */
        for (idx_t state = 0; state < 512; ++state) {
            seq_t cnt = dp_cur[state];
            if (!cnt) continue;
            for (idx_t d = 0; d < 8; ++d) {
                if (_a250386_valid[state][d]) {
                    idx_t nxt = ((size_t)_a250386_b[state] << 6) |
                               ((size_t)_a250386_c[state] << 3) | d;
                    dp_nxt[nxt] += cnt;
                }
            }
        }

        /* swap pointers for the next iteration */
        seq_t *tmp = dp_cur;
        dp_cur = dp_nxt;
        dp_nxt = tmp;

        /* sum all counts for this length */
        seq_t sum = 0;
        for (idx_t i = 0; i < 512; ++i) sum += dp_cur[i];

        /* store result: length = n+3  →  n = len-3 */
        _a250386_cache[len - 3] = sum;

        ++_a250386_computed_n;
        ++_a250386_computed_len;
    }
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* behaviour for n < 1 is irrelevant */
    _a250386_ensure_upto(n);
    return _a250386_cache[n];
}
