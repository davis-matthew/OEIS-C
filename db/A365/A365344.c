/*
 * A365344  a(0) = 0; for n > 0, a(n) is the largest distance squared on a
 *          square spiral between a(n-1) and any previous occurrence of
 *          a(n-1). If a(n-1) has not previously occurred then a(n) = 0.
 * https://oeis.org/A365344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 144, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A365344:  a(0) = 0.
    For n > 0 let v = a(n‑1).  a(n) is the largest squared Euclidean
    distance between the square‑spiral coordinates of n‑1 and any earlier
    index m < n‑1 with a(m) = v;
    if v has not occurred before, a(n) = 0.

    The function memoises all previously computed values and coordinates
    so that repeated calls in any order are fast.  The algorithm is
    O(n²) in the worst case, which is acceptable for n up to ~10⁴.  */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/*  integer square root, floor – used only for ring computation  */
static inline unsigned int _isqrt_idx(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        idx_t sq  = mid * mid;
        if (sq == n) return (unsigned int)mid;
        if (sq < n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return (unsigned int)hi;
}

/*  coordinates of index n in the 0‑based square spiral  */
static void _spiral_coords(idx_t n, int *rx, int *ry)
{
    if (n == 0) {
        *rx = 0; *ry = 0;
        return;
    }
    unsigned int k = _isqrt_idx(n);
    k = (k + 1) / 2;                     /* ring number */
    idx_t start  = (2 * k - 1) * (2 * k - 1);      /* first index of this ring */
    idx_t offset = n - start;             /* 0 … 8k-1 */

    if (offset == 0) {                    /* corner */
        *rx = (int)k;
        *ry = -(int)(k - 1);
        return;
    }

    int up_len   = 2 * k - 1;
    int left_len = 2 * k;
    int down_len = 2 * k;
    int right_len= 2 * k;

    if (offset <= (idx_t)up_len) {                /* upward segment */
        int step = (int)offset;
        *rx = (int)k;
        *ry = -(int)(k - 1) + step;
    } else if (offset <= (idx_t)(up_len + left_len)) { /* leftward */
        int step = (int)(offset - up_len);
        *rx = (int)k - step;
        *ry = (int)k;
    } else if (offset <= (idx_t)(up_len + left_len + down_len)) { /* downward */
        int step = (int)(offset - up_len - left_len);
        *rx = - (int)k;
        *ry = (int)k - step;
    } else {                                    /* rightward */
        int step = (int)(offset - up_len - left_len - down_len);
        *rx = - (int)k + step;
        *ry = - (int)k;
    }
}

/*  memoisation buffers  */
static seq_t * _aval  = NULL;   /* a values   */
static int   * _xarr  = NULL;   /* x‑coords   */
static int   * _yarr  = NULL;   /* y‑coords   */
static size_t  _cap   = 0;      /* allocated capacity */
static idx_t   _last  = (idx_t)-1; /* highest index computed */

/*  ensure capacity for indices 0 … n  */
static void _ensure_capacity(idx_t n)
{
    if (_cap <= (size_t)n) {
        size_t newcap = _cap ? _cap * 2 : 16;
        if (newcap <= (size_t)n) newcap = (size_t)n + 1;
        _aval = (seq_t *)realloc(_aval, newcap * sizeof(seq_t));
        _xarr = (int   *)realloc(_xarr, newcap * sizeof(int));
        _yarr = (int   *)realloc(_yarr, newcap * sizeof(int));
        _cap  = newcap;
    }
}

/*  public function  */
seq_t a(idx_t n)
{
    if (n == 0)            /* a(0) = 0, defined immediately  */
        return (seq_t)0;

    _ensure_capacity(n);

    /* initialise the first element if required  */
    if (_last == (idx_t)-1) {
        _aval[0] = (seq_t)0;
        _xarr[0] = 0;
        _yarr[0] = 0;
        _last    = 0;
    }

    /* compute any missing values up to n  */
    for (idx_t i = _last + 1; i <= n; ++i) {
        idx_t prev = i - 1;
        seq_t v    = _aval[prev];
        seq_t best = 0;

        for (idx_t m = 0; m < prev; ++m) {
            if (_aval[m] == v) {
                long long dx = (long long)_xarr[prev] - _xarr[m];
                long long dy = (long long)_yarr[prev] - _yarr[m];
                long long d2 = dx * dx + dy * dy;
                if ((seq_t)d2 > best) best = (seq_t)d2;
            }
        }

        _aval[i] = best;

        /* compute and store the coordinates of i  */
        int cx, cy;
        _spiral_coords(i, &cx, &cy);
        _xarr[i] = cx;
        _yarr[i] = cy;
    }

    _last = n;
    return _aval[n];
}
