/*
 * A381110  a(n) is the maximum number of points from the set {(k, f(k)); k =
 *          0..n} belonging to a straight line passing through the point (n,
 *          f(n)), where f(n) = A060143(n) = floor(n/phi) and phi is the
 *          golden ratio (sqrt(5)+1)/2.
 * https://oeis.org/A381110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) is the maximum number of points from {(k, f(k)); k=0..n}
 *            lying on a line through (n, f(n)), where f(k)=⌊k/φ⌋, φ=(1+√5)/2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The function is computed by grouping equal slopes
    (f(n)-f(k))/(n-k) for k<n.  */

#include <stdint.h>
#include <stdlib.h>

/* integer square root: floor(sqrt(n)) for 0 ≤ n < 2^64. */
static inline uint64_t isqrt_u64(uint64_t n)
{
    if (n == 0ULL) return 0ULL;
    uint64_t x = n;
    uint64_t y = (x >> 1) + 1ULL;          /* initial guess */
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1U;
    }
    return x;
}

/* Euclidean gcd for 64‑bit unsigned integers. */
static inline uint64_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b != 0ULL) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Helper to compute f(k) = ⌊k/φ⌋ = ⌊(k*√5 – k)/2⌋. */
static inline uint64_t f_value(idx_t k)
{
    uint64_t kk = (uint64_t)k;
    uint64_t prod = 5ULL * kk * kk;              /* k²·5 < 2^64 for k≈10^9 */
    uint64_t s = isqrt_u64(prod);                /* ⌊k*√5⌋ */
    return (s - kk) / 2ULL;
}

/* Slope representation (reduced dy/dx). */
typedef struct {
    int64_t dy;   /* signed difference in f values */
    uint64_t dx;  /* positive difference in indices   */
} slope_t;

/* Comparator for qsort(): compare dy first, then dx. */
static int cmp_slope(const void *a, const void *b)
{
    const slope_t *p1 = (const slope_t *)a;
    const slope_t *p2 = (const slope_t *)b;
    if (p1->dy < p2->dy) return -1;
    if (p1->dy > p2->dy) return 1;
    if (p1->dx < p2->dx) return -1;
    if (p1->dx > p2->dx) return 1;
    return 0;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0U) return 1U;                     /* only point (0,f(0)) */

    uint64_t N = (uint64_t)n;
    uint64_t fn = f_value(n);                   /* f(n) */

    /* Allocate array of slopes for k = 0 .. n-1. */
    size_t len = (size_t)N;                     /* n entries */
    slope_t *s = (slope_t *)malloc(len * sizeof(slope_t));
    if (s == NULL) return 0U;                   /* OOM protection */

    for (size_t i = 0; i < len; ++i) {
        uint64_t k = (uint64_t)i;
        uint64_t fk = f_value(k);

        int64_t dy = (int64_t)fn - (int64_t)fk; /* signed difference */
        uint64_t dx = N - k;                   /* positive difference */

        /* Reduce slope to canonical form. */
        uint64_t g = gcd_u64((uint64_t)(dy >= 0 ? dy : -dy), dx);
        s[i].dy = dy / (int64_t)g;
        s[i].dx = dx / g;
    }

    /* Sort slopes to bring equal ones together. */
    qsort(s, len, sizeof(slope_t), cmp_slope);

    /* Count longest run of identical slopes. */
    size_t maxcnt = 1U;  /* at least one point (k = n itself). */
    size_t cnt = 1U;
    for (size_t i = 1; i < len; ++i) {
        if (s[i].dy == s[i-1].dy && s[i].dx == s[i-1].dx) {
            ++cnt;
        } else {
            if (cnt > maxcnt) maxcnt = cnt;
            cnt = 1U;
        }
    }
    if (cnt > maxcnt) maxcnt = cnt;

    free(s);
    return (seq_t)(maxcnt + 1U);   /* include point (n, f(n)) */
}
