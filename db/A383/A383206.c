/*
 * A383206  Triangle T(n,k), n >= 0, 0 <= k <= n, read by rows, where T(n,k) =
 *          Sum_{j=k..n} 2^(n-j) * Stirling2(n,j) * Stirling2(j,k).
 * https://oeis.org/A383206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where * r = floor((sqrt(8n+1)-1)/2), k = n -
 *            r*(r+1)/2, * T(r,k) = ∑_{j=k..r} 2^{r-j} · S2(r,j) · S2(j,k) *
 *            (S2 = Stirling numbers of the second kind) * * The function
 *            memoises Stirling numbers of the second kind in a * flat 2‑D
 *            array of size (r+1)×(r+1). Only the rows that are need...
 * Range:     u8 n <= 15, u16 n <= 36, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/big_pow.h"   /* provides seq_t big_pow(seq_t base, seq_t exp) */

static idx_t      _max_r   = 0;            /* largest row that has been computed */
static size_t     _stride  = 0;            /* stride of the flat 2‑D array (row+1) */
static seq_t *    _stir    = NULL;          /* cache of Stirling numbers of the second kind */

/* Utility: integer square root (floor) of a 64‑bit value */
static inline idx_t _isqrt(idx_t v)
{
    idx_t low = 0, high = v;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (mid * mid <= v)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Allocate or expand the Stirling cache to include all rows up to 'r'. */
static void _ensure_stirling(idx_t r)
{
    if (_stir == NULL) {                     /* first time: build from scratch */
        size_t new_stride = (size_t)r + 1;
        size_t new_size   = new_stride * new_stride;
        seq_t *new_stir   = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_stir) exit(1);              /* fatal error */

        /* initialise all entries to 0 */
        for (size_t idx = 0; idx < new_size; ++idx)
            new_stir[idx] = 0;

        /* Stirling recurrence S2(n,k) = k*S2(n‑1,k) + S2(n‑1,k‑1) */
        new_stir[0 * new_stride + 0] = 1;    /* S2(0,0) = 1 */
        for (idx_t i = 1; i <= r; ++i) {
            new_stir[i * new_stride + 0] = 0;     /* S2(i,0) = 0 */
            for (idx_t j = 1; j <= i; ++j) {
                seq_t term1 = (seq_t)j * new_stir[(i - 1) * new_stride + j];
                seq_t term2 = new_stir[(i - 1) * new_stride + (j - 1)];
                new_stir[i * new_stride + j] = term1 + term2;
            }
        }
        _stir   = new_stir;
        _stride = new_stride;
        _max_r  = r;
        return;
    }

    if (r <= _max_r) return;                 /* nothing to do */

    /* Expand existing cache */
    size_t new_stride = (size_t)r + 1;
    size_t new_size   = new_stride * new_stride;
    seq_t *new_stir   = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_stir) exit(1);

    /* copy old data */
    for (idx_t i = 0; i <= _max_r; ++i) {
        for (idx_t j = 0; j <= i; ++j) {
            new_stir[i * new_stride + j] = _stir[i * _stride + j];
        }
    }

    /* initialise new entries to 0 */
    for (size_t idx = 0; idx < new_size; ++idx)
        if (idx >= (_max_r + 1) * new_stride) new_stir[idx] = 0;

    /* compute new rows i = _max_r+1 .. r */
    for (idx_t i = _max_r + 1; i <= r; ++i) {
        new_stir[i * new_stride + 0] = 0;
        for (idx_t j = 1; j <= i; ++j) {
            seq_t term1 = (seq_t)j * new_stir[(i - 1) * new_stride + j];
            seq_t term2 = new_stir[(i - 1) * new_stride + (j - 1)];
            new_stir[i * new_stride + j] = term1 + term2;
        }
    }

    free(_stir);
    _stir   = new_stir;
    _stride = new_stride;
    _max_r  = r;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* locate row r, column k for flat index n */
    idx_t temp = _isqrt(8 * n + 1);
    idx_t r    = (temp - 1) >> 1;       /* (temp-1)/2 */
    idx_t start_of_row = r * (r + 1) / 2;
    idx_t k = n - start_of_row;

    /* prepare Stirling numbers up to row r */
    _ensure_stirling(r);

    /* compute T(r,k) */
    seq_t val = 0;
    for (idx_t j = k; j <= r; ++j) {
        seq_t p = big_pow((seq_t)2, (seq_t)(r - j));   /* 2^(r-j) */
        seq_t s1 = _stir[r * _stride + j];
        seq_t s2 = _stir[j * _stride + k];
        val += p * s1 * s2;
    }
    return val;
}
