/*
 * A287166  Smallest number with exactly n representations as a sum of 7
 *          nonzero squares or 0 if no such number exists.
 * https://oeis.org/A287166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A287166 – smallest number with exactly n representations as a sum of 7 non‑zero squares.
   The implementation uses a dynamic DP over partitions into squares.
   The DP is built lazily and extended if a requested n exceeds the current coverage. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h> /* for sqrt, but only for integer computations */

static bool _initialised = false;

/* DP table: dp[k][s] = number of partitions of square–sum s into exactly k squares (unordered) */
static seq_t **_dp = NULL;
static idx_t _dp_size = 0;          /* current maximum sum for which dp is valid */
static int   _last_square = 0;      /* last integer i such that i*i <= _dp_size */

/* Cache of minimal number for each representation count */
static seq_t *_minN = NULL;
static size_t _minN_size = 0;      /* array size, index corresponds to count */

/* Largest representation count seen in current DP */
static size_t _max_count = 0;

/* Helper to reallocate and zero newly added memory */
static void _realloc_and_zero(seq_t **ptr, size_t old_len, size_t new_len) {
    seq_t *tmp = (seq_t *)realloc(*ptr, new_len * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    *ptr = tmp;
    if (new_len > old_len)
        memset(tmp + old_len, 0, (new_len - old_len) * sizeof(seq_t));
}

/* Extend the DP to cover sums up to new_size-1.  Only new squares are processed. */
static void _extend_dp(idx_t new_size) {
    /* Reallocate DP tables to new size */
    size_t old_len = _dp_size + 1;
    _dp_size = new_size - 1;       /* since size is new_size = max_sum+1 */
    for (int k = 0; k <= 7; ++k)
        _realloc_and_zero(&_dp[k], old_len, new_size);

    /* Process new squares that were not handled before */
    int i_start = _last_square + 1;
    int i_end   = (int)floor(sqrt((double)new_size));
    for (int i = i_start; i <= i_end; ++i) {
        idx_t sq = (idx_t)i * (idx_t)i;
        for (idx_t s = sq; s < new_size; ++s) {
            seq_t *dp0 = _dp[0];
            /* k from 1 to 7 */
            for (int k = 1; k <= 7; ++k)
                _dp[k][s] += _dp[k-1][s - sq];
        }
        _last_square = i;
    }

    /* Update minimal number table for new sums */
    for (idx_t s = old_len; s < new_size; ++s) {
        seq_t cnt = _dp[7][s];
        if (cnt == 0) continue;
        size_t idx = (size_t)cnt;
        if (idx >= _minN_size) {
            size_t new_len = idx + 1;
            _realloc_and_zero(&_minN, _minN_size, new_len);
            _minN_size = new_len;
        }
        if (_minN[idx] == 0) _minN[idx] = s;
        if (idx > _max_count) _max_count = idx;
    }
}

/* Lazy initialization of the DP and cache.  Initial size is chosen to cover typical inputs. */
static void _init(void) {
    if (_initialised) return;
    _initialised = true;

    /* Allocate DP tables for sums up to an initial bound (8 000 000 < 2 sec approx) */
    idx_t init_size = 80000;  /* initial maximum N to cover common queries (adjustable) */
    _dp = (seq_t **)malloc(8 * sizeof(seq_t *));
    if (!_dp) exit(EXIT_FAILURE);
    for (int k = 0; k <= 7; ++k) {
        _dp[k] = (seq_t *)calloc(init_size, sizeof(seq_t));
        if (!_dp[k]) exit(EXIT_FAILURE);
    }
    _dp[0][0] = 1;
    _dp_size = init_size - 1;
    _last_square = (int)floor(sqrt((double)init_size));

    /* Process all squares up to init_size */
    for (int i = 1; i <= _last_square; ++i) {
        idx_t sq = (idx_t)i * (idx_t)i;
        for (idx_t s = sq; s <= _dp_size; ++s) {
            for (int k = 1; k <= 7; ++k)
                _dp[k][s] += _dp[k-1][s - sq];
        }
    }

    /* Allocate minimal-number cache */
    _minN = NULL;
    _minN_size = 0;
    _max_count = 0;

    /* Fill cache for initial range */
    for (idx_t s = 1; s <= _dp_size; ++s) {
        seq_t cnt = _dp[7][s];
        if (cnt == 0) continue;
        size_t idx = (size_t)cnt;
        if (idx >= _minN_size) {
            size_t new_len = idx + 1;
            _realloc_and_zero(&_minN, _minN_size, new_len);
            _minN_size = new_len;
        }
        if (_minN[idx] == 0) _minN[idx] = s;
        if (idx > _max_count) _max_count = idx;
    }
}

/* Public function implementing the sequence */
seq_t a(idx_t n) {
    _init();

    /* Ensure DP covers representation count n */
    if ((size_t)n > _max_count) {
        /* Double the DP size until enough counts are seen */
        idx_t new_size = _dp_size * 2 + 1;
        while ((size_t)n > _max_count) {
            _extend_dp(new_size);
            new_size = _dp_size * 2 + 1;
        }
    }

    /* Return stored minimal number or 0 if none */
    if ((size_t)n >= _minN_size) return 0;
    return _minN[n];
}
