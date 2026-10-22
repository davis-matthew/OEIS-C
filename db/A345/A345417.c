/*
 * A345417  Table read by upward antidiagonals: Given m, n >= 1, write
 *          gcd(prime(m),prime(n)) as d = u*prime(m)+v*prime(n) where u, v are
 *          minimal; T(m,n) = u.
 * https://oeis.org/A345417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 78, i16 n <= 78, i32 n <= 78, i64 n <= 78, i128 n <= 78,
 *            big any n
 * Helpers:   ext_gcd, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A345417:  a(n) = the minimal u in
   prime(m)*u + prime(n)*v = gcd(prime(m),prime(n)),
   read from the table T(m,n) in upward‑antidiagonal order.
   The index is offset 1 (a(1) = 0).  For m = n the rule
   prescribes u = 0.  For m ≠ n we solve
     prime(m)*u + prime(n)*v = 1
   and choose the integer pair (u,v) with minimal u²+v².  This
   implementation computes the pair efficiently with the extended
   Euclidean algorithm and a local one‑step optimisation of the
   periodic solution.  The code works for the supplied seq_t and
   idx_t types. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/get_prime.h"
#include "../utils/ext_gcd.h"

static inline uint64_t sumsq(int64_t u, int64_t v)
{
    return (uint64_t)u * (uint64_t)u + (uint64_t)v * (uint64_t)v;
}

seq_t a(idx_t n)
{
    /* Find diagonal t such that T(t-1) < n ≤ T(t),  T(t)=t*(t+1)/2 */
    idx_t t = 1;
    while ((__int128)t * (t + 1) / 2 < n) {
        ++t;
    }
    idx_t prev_tri = (__int128)(t - 1) * t / 2;
    idx_t pos = n - prev_tri - 1;          /* 0‑based position on the diagonal */
    idx_t m = t - pos;                     /* 1‑based row index */
    idx_t k = pos + 1;                     /* 1‑based column index */

    if (m == k) {                          /* diagonal entry: u = 0 */
        return (seq_t)0;
    }

    uint32_t pm = get_prime(m);            /* prime(m) */
    uint32_t pk = get_prime(k);            /* prime(k) */

    int64_t inv, dummy;
    ext_gcd((int64_t)pm, (int64_t)pk, &inv, &dummy);
    inv %= (int64_t)pk;
    if (inv < 0) inv += (int64_t)pk;       /* modular inverse of pm modulo pk */

    int64_t u0 = inv;                      /* one particular solution of pm*u + pk*v = 1 */
    int64_t v0 = (1 - (int64_t)pm * u0) / (int64_t)pk;

    /* Period of solutions: (u,v) = (u0 + pk*t , v0 - pm*t) */
    int64_t num = -( (int64_t)pk * u0 - (int64_t)pm * v0 );
    int64_t den = (int64_t)pk * pk + (int64_t)pm * pm;
    int64_t t0 = num / den;                 /* integer division (truncs toward 0) */

    int64_t best_u = u0 + (int64_t)pk * t0;
    int64_t best_v = v0 - (int64_t)pm * t0;
    uint64_t best_sum = sumsq(best_u, best_v);

    /* Check the neighbouring t values (t0-1, t0, t0+1, t0+2) to be safe */
    for (int d = -1; d <= 2; ++d) {
        int64_t t = t0 + d;
        int64_t u = u0 + (int64_t)pk * t;
        int64_t v = v0 - (int64_t)pm * t;
        uint64_t s = sumsq(u, v);
        if (s < best_sum) {
            best_sum = s;
            best_u = u;
            best_v = v;
        }
    }

    return (seq_t)best_u;
}
