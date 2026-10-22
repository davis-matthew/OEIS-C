/*
 * A357728  Square array T(n,k), n>=0, k>=0, read by antidiagonals, where
 *          column k is the expansion of e.g.f. cos( sqrt(k) * (exp(x) - 1) ).
 * https://oeis.org/A357728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(i, j) where n = d(d+1)/2 + i, j = d - i, * and T(n, k)
 *            = sum_{j=0..⌊n/2⌋} (-k)^j * Stirling2(n, 2j)
 * Range:     i8 n <= 41, i16 n <= 87, i32 n <= 185, i64 n <= 430,
 *            i128 n <= 1074, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

#define MAX_STIRLING_N 150

static bool _stirling_initialized = false;
static seq_t _stirling[MAX_STIRLING_N + 1][MAX_STIRLING_N + 1];
static size_t _current_stirling_n = 0;

/* Ensure Stirling numbers S[n][m] are computed up to n = needed_n. */
static void _ensure_stirling(size_t needed_n) {
    if (_stirling_initialized && needed_n <= _current_stirling_n) return;
    if (!_stirling_initialized) {
        /* Zero-initialize the table. */
        for (size_t i = 0; i <= MAX_STIRLING_N; ++i)
            for (size_t j = 0; j <= MAX_STIRLING_N; ++j)
                _stirling[i][j] = 0;
        _stirling[0][0] = 1;
        _current_stirling_n = 0;
        _stirling_initialized = true;
    }
    for (size_t n = _current_stirling_n + 1; n <= needed_n; ++n) {
        for (size_t m = 0; m <= n; ++m) {
            if (m == 0) {
                _stirling[n][m] = 0;
            } else if (m == n) {
                _stirling[n][m] = 1;
            } else {
                seq_t term1 = (seq_t)m * _stirling[n - 1][m];
                seq_t term2 = _stirling[n - 1][m - 1];
                _stirling[n][m] = term1 + term2;
            }
        }
    }
    _current_stirling_n = needed_n;
}

/* Evaluate T(n, k) using the stirling sum. */
static seq_t _T_value(idx_t n, idx_t k) {
    _ensure_stirling((size_t)n);
    seq_t result = 0;
    seq_t pow_k = 1;   /* k^j */
    size_t max_j = (size_t)(n / 2);
    for (size_t j = 0; j <= max_j; ++j) {
        size_t m = 2 * j;
        seq_t term = pow_k * _stirling[n][m];
        if (j & 1U) {
            result -= term; /* (-1)^j */
        } else {
            result += term;
        }
        pow_k *= (seq_t)k;  /* prepare k^(j+1) */
    }
    return result;
}

/* Compute the nth term of sequence A357728. */
seq_t a(idx_t n) {
    /* Find the diagonal d such that d(d+1)/2 <= n < (d+1)(d+2)/2 */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= n)
        ++d;
    idx_t tri = d * (d + 1) / 2;      /* first index on this diagonal */
    idx_t offset = n - tri;           /* step inside the diagonal */
    idx_t i = offset;                 /* row index */
    idx_t j = d - offset;             /* column index */
    return _T_value(i, j);
}
