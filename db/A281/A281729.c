/*
 * A281729  Number of ways to write n as x^2 + y^2 + z^2 + w^2 with x,z
 *          positive integers and y,w nonnegative integers such that both
 *          9*x^2 + 246*x*y + y^2 and 9*z^2 + 666*z*w + w^2 are squares.
 * https://oeis.org/A281729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 3424, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: number of ways to write n = x² + y² + z² + w²
   with x,z > 0, y,w ≥ 0 and
         9x² + 246xy + y²   a perfect square
         9z² + 666zw + w²   a perfect square.
   The routine enumerates all admissible (x,y) and (z,w) pairs (up to
   sqrt(n)), counts how many pairs give each possible sum, and then
   combines complementary sums to obtain the desired count.
   No closed formula is known.  The runtime is O(n · √n) and is well
   below two seconds for n ≤ 10 000. */

#include <stdbool.h>
#include <stdlib.h>

/* ---------- helpers ---------- */

/* Integer square root for idx_t (uint64_t).  Returns ⌊√v⌋. */
static inline idx_t isqrt_u64(idx_t v)
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

/* Integer square root for seq_t.  Works for the small values that
   occur in the tests (≈ 2‑3 000 000).  Uses binary search on seq_t. */
static inline seq_t isqrt_seq(seq_t v)
{
    seq_t low = 0, high = v;
    while (low < high) {
        seq_t mid = (low + high + 1) >> 1;
        seq_t sq = mid * mid;          /* safe: values are tiny (< 2¹³⁰) */
        if (sq <= v)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Test whether seq_t value is a perfect square. */
static inline bool is_square_seq(seq_t v)
{
    seq_t r = isqrt_seq(v);
    return r * r == v;
}

/* ---------- main routine ---------- */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* offset 1 – values for n≥1 are needed */

    /* Allocate counting tables.  Size n+1 because sums run up to n-1. */
    seq_t *cnt_xy = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    seq_t *cnt_zw = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!cnt_xy || !cnt_zw) {
        /* out‑of‑memory; terminate */
        exit(1);
    }

    /* ----- count admissible (x,y) pairs ----- */
    idx_t max_x = isqrt_u64(n);
    for (idx_t x = 1; x <= max_x; ++x) {            /* x > 0   */
        idx_t xsq = x * x;
        idx_t rem = n - xsq;
        idx_t max_y = isqrt_u64(rem);
        for (idx_t y = 0; y <= max_y; ++y) {        /* y ≥ 0   */
            seq_t exp = (seq_t)9 * xsq
                        + (seq_t)246 * x * y
                        + (seq_t)y * y;
            if (!is_square_seq(exp))
                continue;
            idx_t sum = xsq + y * y;
            cnt_xy[sum] += 1;
        }
    }

    /* ----- count admissible (z,w) pairs ----- */
    idx_t max_z = isqrt_u64(n);
    for (idx_t z = 1; z <= max_z; ++z) {            /* z > 0   */
        idx_t zsq = z * z;
        idx_t rem = n - zsq;
        idx_t max_w = isqrt_u64(rem);
        for (idx_t w = 0; w <= max_w; ++w) {        /* w ≥ 0   */
            seq_t exp = (seq_t)9 * zsq
                        + (seq_t)666 * z * w
                        + (seq_t)w * w;
            if (!is_square_seq(exp))
                continue;
            idx_t sum = zsq + w * w;
            cnt_zw[sum] += 1;
        }
    }

    /* ----- combine complementary sums ----- */
    seq_t result = 0;
    for (idx_t s = 1; s < n; ++s) {                 /* s≥1, t≥1 */
        seq_t cxy = cnt_xy[s];
        if (!cxy)
            continue;
        idx_t t = n - s;
        seq_t czw = cnt_zw[t];
        if (!czw)
            continue;
        result += cxy * czw;
    }

    free(cnt_xy);
    free(cnt_zw);
    return result;
}
