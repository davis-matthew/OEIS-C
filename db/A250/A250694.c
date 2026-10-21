/*
 * A250694  Number of (3+1)X(n+1) 0..3 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 11, u64 n <= 27, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A250694: number of 4×(n+1) 0..3 arrays with nondecreasing horizontal
   differences and nondecreasing absolute vertical differences.
   The code implements dynamic programming on column states:
   - Build a transition table between 256 possible column states.
   - dp[j+1][t] = Σ dp[j][s] over all s that can precede t
     (i.e. horizontal differences are nondecreasing and
     |vertical diff(t,i)| ≥ |vertical diff(s,i)| for i=1..3).
   - a(n) = Σ_t dp[n][t]   (n ≥ 1).  All other indices are undefined. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Transition table */
static int _trans_next[256][256];
static uint8_t _trans_cnt[256];
static uint8_t _d0[256], _d1[256], _d2[256], _d3[256];

/* DP arrays */
static seq_t _dp_cur[256];
static seq_t _dp_next[256];

/* Memoized answers: a[0] = a(1), a[1] = a(2), ... */
static seq_t *_answers = NULL;
static size_t _answers_cap = 0;
static size_t _computed_n = 0;   /* number of stored terms */

/* One‑time initialisation of tables */
static void _init(void)
{
    static bool _initialized = false;
    if (_initialized) return;
    _initialized = true;

    /* Pre‑compute digits for each column state */
    for (int s = 0; s < 256; ++s) {
        _d0[s] = s & 3;
        _d1[s] = (s >> 2) & 3;
        _d2[s] = (s >> 4) & 3;
        _d3[s] = (s >> 6) & 3;
    }

    /* Build transition lists */
    for (int p = 0; p < 256; ++p) {
        int p0 = _d0[p], p1 = _d1[p], p2 = _d2[p], p3 = _d3[p];
        int vprev1 = (p1 >= p0) ? p1 - p0 : p0 - p1;
        int vprev2 = (p2 >= p1) ? p2 - p1 : p1 - p2;
        int vprev3 = (p3 >= p2) ? p3 - p2 : p2 - p3;
        int cnt = 0;
        for (int s = 0; s < 256; ++s) {
            int s0 = _d0[s], s1 = _d1[s], s2 = _d2[s], s3 = _d3[s];
            int diff0 = s0 - p0;
            int diff1 = s1 - p1;
            int diff2 = s2 - p2;
            int diff3 = s3 - p3;
            if (diff0 > diff1 || diff1 > diff2 || diff2 > diff3) continue;
            int vcurr1 = (s1 >= s0) ? s1 - s0 : s0 - s1;
            int vcurr2 = (s2 >= s1) ? s2 - s1 : s1 - s2;
            int vcurr3 = (s3 >= s2) ? s3 - s2 : s2 - s3;
            if (vcurr1 < vprev1 || vcurr2 < vprev2 || vcurr3 < vprev3) continue;
            _trans_next[p][cnt++] = s;
        }
        _trans_cnt[p] = (uint8_t)cnt;
    }

    /* Initialise dp for the first column: all 256 states are allowed */
    for (int s = 0; s < 256; ++s) {
        _dp_cur[s] = (seq_t)1;
    }
}

/* Public interface required by the template */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for indices < 1 */

    _init();

    /* Extend computed terms up to n */
    for (size_t step = _computed_n; step < (size_t)n; ++step) {
        /* Zero the next dp array */
        for (int s = 0; s < 256; ++s) _dp_next[s] = (seq_t)0;

        /* Update via transitions */
        for (int p = 0; p < 256; ++p) {
            seq_t valp = _dp_cur[p];
            if (valp == (seq_t)0) continue;
            int cnt = _trans_cnt[p];
            for (int i = 0; i < cnt; ++i) {
                int t = _trans_next[p][i];
                _dp_next[t] += valp;
            }
        }

        /* Compute the total for this n */
        seq_t total = (seq_t)0;
        for (int s = 0; s < 256; ++s) total += _dp_next[s];

        /* Store answer, growing array if necessary */
        if (_computed_n >= _answers_cap) {
            size_t new_cap = _answers_cap ? _answers_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(_answers, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            _answers = tmp;
            _answers_cap = new_cap;
        }
        _answers[_computed_n] = total;
        ++_computed_n;

        /* Prepare for next iteration: copy next DP to current */
        for (int s = 0; s < 256; ++s) _dp_cur[s] = _dp_next[s];
    }

    return _answers[(size_t)n - 1];
}
