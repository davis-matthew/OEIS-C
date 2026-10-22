/*
 * A345415  Table read by upward antidiagonals: Given m, n >= 1, write
 *          gcd(m,n) as d = u*m+v*n where u, v are minimal; T(m,n) = u.
 * https://oeis.org/A345415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 91, i16 n <= 91, i32 n <= 91, i64 n <= 91, i128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Value: A345415 – minimal u in d = u*m + v*n with u²+v² minimal (upward antidiagonals) */

#include <stdint.h>

/* ---------- integer square root ---------- */
static inline idx_t isqrt_u64(idx_t n)   /* floor(sqrt(n)) */
{
    idx_t low = 0, high = n >> 1 | 1;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        __int128 prod = (__int128)mid * mid;
        if (prod <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* ---------- extended Euclidean algorithm ---------- */
static inline int64_t egcd(long long a, long long b, long long *x, long long *y)
{
    if (b == 0) {
        *x = 1; *y = 0;
        return a;
    }
    long long x1, y1;
    int64_t g = egcd(b, a % b, &x1, &y1);
    *x = y1;
    *y = x1 - (a / b) * y1;
    return g;
}

/* ---------- floor division for possibly negative numerator ---------- */
static inline long long floor_div(__int128 num, __int128 den)
{
    long long q = (long long)(num / den);          /* truncates toward zero */
    __int128 r = num % den;
    if ((num ^ den) < 0 && r != 0)                 /* adjust for negative */
        q--;
    return q;
}

/* ---------- compute u for given m,n ---------- */
static inline seq_t compute_u(uint64_t m, uint64_t n)
{
    if (m == n)                 /* special rule for diagonal entries */
        return 0;
    long long ml = (long long)m;
    long long nl = (long long)n;

    long long x0, y0;
    int64_t g = egcd(ml, nl, &x0, &y0);       /* ml*x0 + nl*y0 = g */

    long long a = nl / g;                     /* shift step for u */
    long long b = ml / g;                     /* shift step for v */

    /* find t that minimises (x0 + t*a)² + (y0 - t*b)² */
    __int128 num = -((__int128)x0 * a - (__int128)y0 * b);
    __int128 den = (__int128)a * a + (__int128)b * b;

    long long t0 = floor_div(num, den);       /* floor of real minimiser */
    long long best_t = t0;
    long long best_u = x0 + t0 * a;
    /* check the neighbour with t0+1 (only two possibilities suffice) */
    long long t1 = t0 + 1;
    long long u1 = x0 + t1 * a;
    __int128 dist0 = (__int128)(best_u) * best_u + (__int128)(y0 - t0 * b) * (y0 - t0 * b);
    __int128 dist1 = (__int128)(u1) * u1 + (__int128)(y0 - t1 * b) * (y0 - t1 * b);
    if (dist1 < dist0) {
        best_t = t1;
        best_u = u1;
    }

    return (seq_t)best_u;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    /* map index n to position (m,n) on upward antidiagonals */
    idx_t disc   = 1 + 8 * n;
    idx_t s      = isqrt_u64(disc);
    idx_t k      = (s - 1) / 2;                    /* rough estimate */
    if ((__int128)k * (k + 1) / 2 < n)
        ++k;                                      /* ensure k(k+1)/2 >= n */

    idx_t prev = (__int128)k * (k - 1) / 2;        /* sum of earlier rows */
    idx_t t     = n - prev - 1;                   /* 0‑based offset within diagonal */

    uint64_t m  = k - t;                          /* row index */
    uint64_t nc = t + 1;                          /* column index */

    return compute_u(m, nc);
}
