/*
 * A347735  Square array T(n, k), n, k > 0, read by antidiagonals; let b be
 *          the function that associates to any pair of integers (u, v) the
 *          Bézout coefficients (x, y) as produced by the extended Euclidean
 *          algorithm (u*x + v*y = gcd(u, v)); T(n, k) is the number of
 *          iterations of b when starting from (n, k) ne...
 * https://oeis.org/A347735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Iterates reduced Bézout coefficient pairs until a unit vector is reached.
   The sequence a(n) is obtained by reading the square array T(n,k) by
   antidiagonals:  a(n) = T(i,j) where (i,j) is the pair corresponding
   to index n via the mapping described below.  T(i,j) counts the
   number of iterations of the extended Euclidean algorithm needed
   to obtain a unit vector (±1,0) or (0,±1). */
#include <stdint.h>
#include <math.h>
#include <stdbool.h>

/* Compute Bézout coefficients for (a,b)
   returning x,y such that a*x + b*y = gcd(a,b). */
static void egcd(long long a, long long b, long long *x, long long *y)
{
    long long x0 = 1, y0 = 0, x1 = 0, y1 = 1;
    long long aa = a >= 0 ? a : -a;
    long long bb = b >= 0 ? b : -b;
    while (bb != 0) {
        long long q = aa / bb;
        long long r = aa - q * bb;
        long long tmp = x0 - q * x1;
        long long tmpy = y0 - q * y1;
        x0 = x1; y0 = y1;
        x1 = tmp; y1 = tmpy;
        aa = bb; bb = r;
    }
    /* Adjust signs to satisfy the original equation */
    if (a < 0) x0 = -x0;
    if (b < 0) y0 = -y0;
    *x = x0;
    *y = y0;
}

/* Count iterations needed for (u,v) to become a unit vector. */
static int iter_count(long long u, long long v)
{
    int cnt = 0;
    while (true) {
        long long x, y;
        egcd(u, v, &x, &y);
        ++cnt;
        if ((x == 1 || x == -1) && y == 0) return cnt;
        if ((y == 1 || y == -1) && x == 0) return cnt;
        u = x;
        v = y;
    }
}

/* triangular number T(m) = m*(m+1)/2 using 128‑bit arithmetic to avoid overflow */
static inline unsigned __int128 triangular(idx_t m)
{
    return (unsigned __int128)m * (m + 1) / 2;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Find the smallest m such that triangular(m) >= n.  Use a floating
       point estimate and correct with a few adjustments to avoid expensive
       linear search for large n. */
    idx_t m = (idx_t)((sqrt(8.0L * (long double)n + 1.0L) - 1.0L) / 2.0L);
    if (triangular(m) < n) {
        while (triangular(m) < n) ++m;
    } else {
        while (m > 0 && triangular(m - 1) >= n) --m;
    }

    idx_t sum = m + 1;
    idx_t prev_total = triangular(m - 1); /* number of entries before this diagonal */
    idx_t k = n - prev_total;           /* position within the current diagonal (1‑based) */
    idx_t i = k;
    idx_t j = sum - k;

    /* Compute the iteration count for the pair (i,j) */
    int cnt = iter_count((long long)i, (long long)j);
    return (seq_t)cnt;
}
