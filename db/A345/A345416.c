/*
 * A345416  Table read by upward antidiagonals: Given m, n >= 1, write
 *          gcd(m,n) as d = u*m+v*n where u, v are minimal; T(m,n) = v.
 * https://oeis.org/A345416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = v, where for the coordinates (m,n) obtained by reading
 *            the positive integer lattice points up‑ward along antidiagonals,
 *            the gcd d = gcd(m,n) is written as d = u*m + v*n, the pair (u,v)
 *            minimising u² + v² is chosen (with u=0,v=1 when m=n). The
 *            function returns this coefficient v
 * Range:     i8 n <= 91, i16 n <= 91, i32 n <= 91, i64 n <= 91, i128 n <= 91,
 *            big any n
 * Helpers:   floor_div_ll, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/floor_div_ll.h"

/* extended Euclidean algorithm: for coprime a,b returns one pair (x,y) such that a*x + b*y = 1 */
static void extended_gcd(seq_t a, seq_t b, seq_t *x, seq_t *y)
{
    seq_t old_r = a, r = b;
    seq_t old_s = 1, s = 0;
    seq_t old_t = 0, t = 1;
    while (r != (seq_t)0) {
        seq_t q = old_r / r;
        seq_t tmp = r; r = old_r - q * r; old_r = tmp;
        tmp = s; s = old_s - q * s; old_s = tmp;
        tmp = t; t = old_t - q * t; old_t = tmp;
    }
    *x = old_s;
    *y = old_t;
    /* old_r is gcd(a,b) which is 1 for coprime a,b */
}

/* map linear index n (>=1) to lattice coordinates (m,n2) read along upward
   antidiagonals.  m >= 1, n2 >= 1. */
static void index_to_pair(idx_t n, idx_t *m, idx_t *n2)
{
    /* Find smallest k with k*(k+1)/2 >= n (k is the diagonal length). */
    idx_t k = 1;
    while ((k * (k + 1)) / 2 < n) {
        ++k;
    }
    idx_t prev = ((k - 1) * k) / 2;      /* total elements before diagonal k */
    idx_t offset = n - prev - (idx_t)1;  /* 0‑based position along this diagonal */
    *m   = k - offset;                   /* largest m  first */
    *n2  = offset + (idx_t)1;            /* n grows */
}

seq_t a(idx_t n)
{
    idx_t m, n2;
    index_to_pair(n, &m, &n2);

    /* If m == n2, by convention u = 0, v = 1. */
    if (m == n2) {
        return (seq_t)1;
    }

    /* Compute the reduced coprime pair. */
    seq_t d  = gcd((seq_t)m, (seq_t)n2);
    seq_t m1 = (seq_t)m / d;
    seq_t n1 = (seq_t)n2 / d;

    /* Find one Bézout solution. */
    seq_t x0, y0;
    extended_gcd(m1, n1, &x0, &y0);   /* m1*x0 + n1*y0 = 1 */

    /* Convert to signed long long for the small arithmetic below. */
    long long m1l = (long long)m1;
    long long n1l = (long long)n1;
    long long x0l = (long long)x0;
    long long y0l = (long long)y0;

    /* Find the integer k that minimises (x0 + k*n1)^2 + (y0 - k*m1)^2. */
    long long numerator   = n1l * x0l - m1l * y0l;
    long long denominator = n1l * n1l + m1l * m1l;
    long long k0 = floor_div_ll(numerator, denominator);

    /* Evaluate at the nearest integers to k0. */
    long long best_f = -1;
    long long best_y = 0;
    for (long long dk = -1; dk <= 1; ++dk) {
        long long k = k0 + dk;
        long long x = x0l + k * n1l;
        long long y = y0l - k * m1l;
        long long f = x * x + y * y;
        if (best_f == -1 || f < best_f) {
            best_f = f;
            best_y = y;
        }
    }

    return (seq_t)best_y;
}
