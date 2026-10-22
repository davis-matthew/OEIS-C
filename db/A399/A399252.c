/*
 * A399252  Rectangular array read by descending antidiagonals: r(n, k) =
 *          floor(k*x^n), where x = -1+sqrt(5).
 * https://oeis.org/A399252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy, changed
 * Formula:   a(n) is the term read from the array r(r,c) = floor(c*x^r) with
 *            x = -1 + sqrt(5), using descending antidiagonals. r and c are
 *            obtained from n by traversing the antidiagonals
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* integer square root of a 128‑bit number, floor value (returns uint64_t) */
static inline unsigned long long isqrt128(unsigned __int128 v) {
    unsigned long long lo = 0, hi = 0xFFFFFFFFFFFFFFFFULL;
    unsigned long long ans = 0;
    while (lo <= hi) {
        unsigned long long mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= v) {
            ans = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return ans;
}

/* floor(p * sqrt(5)) for non‑negative integer p */
static inline long long floor_sqrt5_nonneg(unsigned long long p) {
    unsigned __int128 p128 = (unsigned __int128)p;
    unsigned __int128 v = p128 * p128 * 5;   /* v = 5 * p^2 */
    return (long long)isqrt128(v);
}

/* global caches for the coefficients of α^n = x_n + y_n * sqrt(5) */
static int64_t *x_cache = NULL;
static int64_t *y_cache = NULL;
static size_t cache_len = 0;

/* enlarge cache to at least new_len (including 0 index) */
static void ensure_cache(size_t new_len) {
    if (new_len <= cache_len) return;
    int64_t *new_x = (int64_t *)malloc(new_len * sizeof(int64_t));
    int64_t *new_y = (int64_t *)malloc(new_len * sizeof(int64_t));
    /* compute from scratch */
    new_x[0] = 1;         /* α^0 = 1 */
    new_y[0] = 0;
    for (size_t i = 1; i < new_len; ++i) {
        /* recurrence: α^i = (-x_{i-1} + 5*y_{i-1}) + (x_{i-1} - y_{i-1})*sqrt5 */
        new_x[i] = -new_x[i-1] + 5 * new_y[i-1];
        new_y[i] =  new_x[i-1] - new_y[i-1];
    }
    free(x_cache);
    free(y_cache);
    x_cache = new_x;
    y_cache = new_y;
    cache_len = new_len;
}

/* main sequence function */
seq_t a(idx_t n) {
    if (n == 0) {  /* offset is 1, but guard for safety */
        return (seq_t)0;
    }

    /* find diagonal index u such that u*(u+1)/2 >= n   (u >= 1) */
    size_t low = 0, high = n;   /* n fits in size_t for the intended limits */
    size_t u = 0;
    while (low <= high) {
        size_t mid = low + ((high - low) >> 1);
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t >= n) {
            u = mid;
            if (mid == 0) break;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    if (u == 0) u = 1;  /* safety */

    /* preceding cumulative terms */
    unsigned __int128 prev_cum128 = (unsigned __int128)(u - 1) * u / 2;
    size_t prev_cum = (size_t)prev_cum128;  /* fits in size_t for typical n */
    size_t offset = (size_t)n - prev_cum - 1;   /* 0‑based within diagonal */

    size_t r = offset + 1;                 /* row index   (>=1) */
    size_t c = u - offset;                 /* column index (>=1) */

    /* ensure coefficients for row r computed */
    ensure_cache(r + 1);
    int64_t x_r = x_cache[r];
    int64_t y_r = y_cache[r];

    /* compute floor(c * α^r) = c*x_r + floor(c*y_r * sqrt5) */
    long long prod_int = (long long)c * x_r;            /* integer part */
    long long p = (long long)c * y_r;                  /* scaled irrational part */

    long long term;
    if (p >= 0) {
        term = floor_sqrt5_nonneg((unsigned long long)p);
    } else {
        unsigned long long abs_p = (unsigned long long)(-p);
        long long sq = floor_sqrt5_nonneg(abs_p);
        term = -(sq + 1);        /* floor of negative value */
    }

    long long result = prod_int + term;
    return (seq_t)result;
}
