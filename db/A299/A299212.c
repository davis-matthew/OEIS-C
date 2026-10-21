/*
 * A299212  Expansion of 1/(1 - x*Product_{k>=1} 1/(1 + x^k)^k).
 * https://oeis.org/A299212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 10, i16 n <= 21, i32 n <= 40, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A299212
   a(0) = 1
   a(n) = ∑_{k=1..n} c[k‑1] * a(n‑k)    for n ≥ 1
   c[0] = 1
   n * c[n] = ∑_{k=1..n} e[k] * c[n‑k]
   e[k] = ∑_{d|k} d^2 * (−1)^{k/d}        (integer values)   */

/* This implementation memoises the two auxiliary sequences
   e[·] (int64_t) and c[·] (seq_t) and then builds a[·].
   All dynamic memory is obtained with malloc/realloc, cast to
   the appropriate type so that it compiles in both C and C++.
   The code respects the requested signatures and is efficient
   for n up to about 100 000. */

#include <stdlib.h>

static idx_t _a_max_n     = 0;      /* highest n computed for a() */
static seq_t *_a_cache    = NULL;   /* a values */
static size_t _a_cap      = 0;

static idx_t _e_cap       = 0;      /* e array size (≥ needed+1) */
static int64_t *_e_cache  = NULL;   /* e[k] = Σ_{d|k} d² (−1)^{k/d} */

static idx_t _c_cap       = 0;      /* c array size (≥ needed) */
static seq_t *_c_cache    = NULL;   /* c[n] values */

/* Ensure that e[1..n] are computed.  Returns pointer to e[0]..e[n]. */
static int64_t* ensure_e(idx_t n)
{
    if (n + 1 > _e_cap) {
        size_t new_cap = (size_t)n + 1;
        int64_t *tmp = (int64_t*)realloc(_e_cache, new_cap * sizeof(int64_t));
        if (!tmp) exit(1);
        _e_cache = tmp;
        for (idx_t k = _e_cap; k <= n; ++k) {
            int64_t sum = 0;
            for (idx_t d = 1; d * d <= k; ++d) {
                if (k % d == 0) {
                    idx_t d1 = d;
                    idx_t m1 = k / d1;
                    int sign1 = (m1 & 1) ? -1 : 1;
                    sum += (int64_t)d1 * d1 * sign1;

                    idx_t d2 = k / d;
                    if (d2 != d1) {
                        idx_t m2 = k / d2;   /* which is d1 */
                        int sign2 = (m2 & 1) ? -1 : 1;
                        sum += (int64_t)d2 * d2 * sign2;
                    }
                }
            }
            _e_cache[k] = sum;
        }
        _e_cache[0] = 0;
        _e_cap = new_cap;
    }
    return _e_cache;
}

/* Ensure that c[0..max_n] are computed.  Returns pointer to c[0]. */
static seq_t* ensure_c(idx_t max_n)
{
    if (max_n == 0) {
        if (_c_cap == 0) {
            _c_cache = (seq_t*)malloc(sizeof(seq_t));
            if (!_c_cache) exit(1);
            _c_cache[0] = (seq_t)1;
            _c_cap = 1;
        }
        return _c_cache;
    }
    if (max_n + 1 > _c_cap) {
        size_t new_cap = (size_t)max_n + 1;
        seq_t *tmp = (seq_t*)realloc(_c_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        _c_cache = tmp;
        if (_c_cap == 0) {
            _c_cache[0] = (seq_t)1;
            _c_cap = 1;
        }
        idx_t old_cap = (idx_t)_c_cap;
        int64_t *e = ensure_e(max_n);
        for (idx_t n = old_cap; n <= max_n; ++n) {
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= n; ++k) {
                sum += (seq_t)e[k] * _c_cache[n - k];
            }
            _c_cache[n] = sum / (seq_t)n;   /* integer division */
        }
        _c_cap = new_cap;
    }
    return _c_cache;
}

/* Ensure that a[0..max_n] are computed.  Returns pointer to a[0]. */
static seq_t* ensure_a(idx_t max_n)
{
    if (max_n + 1 > _a_cap) {
        size_t new_cap = (size_t)max_n + 1;
        seq_t *tmp = (seq_t*)realloc(_a_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        _a_cache = tmp;
        if (_a_cap == 0) {
            _a_cache[0] = (seq_t)1;
            _a_cap = 1;
        }
        idx_t old_cap = (idx_t)_a_cap;
        seq_t *c = ensure_c(max_n - 1);      /* need c[0..max_n-1] */
        for (idx_t n = old_cap; n <= max_n; ++n) {
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= n; ++k) {
                sum += c[k - 1] * _a_cache[n - k];
            }
            _a_cache[n] = sum;
        }
        _a_cap = new_cap;
    }
    return _a_cache;
}

/* Main entry point for the sequence. */
seq_t a(idx_t n)
{
    if (n > _a_max_n) _a_max_n = n;
    seq_t *arr = ensure_a(_a_max_n);
    return arr[n];
}
