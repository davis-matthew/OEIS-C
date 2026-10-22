/*
 * A345418  Table read by upward antidiagonals: Given m, n >= 1, write
 *          gcd(prime(m),prime(n)) as d = u*prime(m)+v*prime(n) where u, v are
 *          minimal; T(m,n) = v.
 * https://oeis.org/A345418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 78, i32 n <= 78, i64 n <= 78, i128 n <= 78,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A345418:
   a(n) = minimal v such that for the pair (m,p) read by upward
   antidiagonals of the prime table,
      gcd(prime(m),prime(p)) = u*prime(m)+v*prime(p),
   with u,v chosen to minimize u²+v².  On the diagonal we set
   u=0, v=1.  The antidiagonal mapping is given by the OEIS
   conventions (n starts at 1). */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

/* integer square‑root floor for 64‑bit unsigned integers */
static idx_t isqrt_floor(idx_t x)
{
    if (x < 2) return x;
    idx_t lo = 1, hi = x >> 1;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        idx_t sq = mid * mid;
        if (sq == x) return mid;
        if (sq < x) lo = mid + 1;
        else hi = mid - 1;
    }
    return hi;
}

/* Extended Euclidean algorithm: returns x,y such that
   a*x + b*y = gcd(a,b).  a,b > 0. */
static void ext_gcd(seq_t a, seq_t b, seq_t *x, seq_t *y)
{
    seq_t x0 = 1, y0 = 0, x1 = 0, y1 = 1;
    while (b != 0) {
        seq_t q = a / b;
        seq_t r = a - q * b;
        seq_t x2 = x0 - q * x1;
        seq_t y2 = y0 - q * y1;
        a = b; b = r;
        x0 = x1; x1 = x2;
        y0 = y1; y1 = y2;
    }
    *x = x0; *y = y0;
}

/* Find the minimal v for coprime primes a,b.  Uses the standard
   shift technique that tests positions near the continuous optimum. */
static seq_t minimal_v(seq_t a, seq_t b)
{
    seq_t u0, v0;
    ext_gcd(a, b, &u0, &v0);          /* u0*a + v0*b = 1 */

    /* dot = u0*b + v0*a, denom = a^2+b^2 */
    seq_t dot = u0 * b + v0 * a;
    seq_t denom = a * a + b * b;

    /* approximate optimal shift k0 ≈ -dot/denom */
    seq_t k0 = -(dot / denom);
    int64_t k0i = (int64_t)k0;        /* safe: |k0| < 1 for the ranges used */

    seq_t best_v = v0;
    seq_t best_score;
    bool first = true;

    for (int64_t dk = -2; dk <= 2; ++dk) {
        int64_t k = k0i + dk;
        seq_t k_seq = (seq_t)k;
        seq_t u = u0 + k_seq * b;
        seq_t v = v0 - k_seq * a;
        seq_t score = u * u + v * v;
        if (first || score < best_score) {
            best_score = score;
            best_v = v;
            first = false;
        }
    }
    return best_v;
}

/* Return the n‑th term of A345418 (n is 1‑based). */
seq_t a(idx_t n)
{
    /* determine (m,p) from the upward antidiagonal representation */
    idx_t k = (isqrt_floor(1 + 8 * n) - 1) / 2;
    if (k * (k + 1) / 2 < n) k++;   /* ensure k is the correct diagonal index */

    idx_t prev = (k - 1) * k / 2;
    idx_t offset = n - prev - 1;

    idx_t m = k - offset;          /* 1‑based index of first prime */
    idx_t p = offset + 1;          /* 1‑based index of second prime */

    if (m == p) {
        /* diagonal: minimal representation is u=0, v=1 */
        return 1;
    }

    seq_t prime_m = (seq_t)get_prime(m);
    seq_t prime_p = (seq_t)get_prime(p);

    return minimal_v(prime_m, prime_p);
}
