/*
 * A281385  Triangular array T(n, k) = n^2 + n*k - k^2.
 * https://oeis.org/A281385
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 122, u16 n <= 9098, u32 n <= 9098, u64 n <= 9098,
 *            u128 n <= 9098, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm for A281385:
 *  The sequence is obtained by reading the triangular
 *  array T(n,k) = n² + n*k – k² row by row.
 *
 *  For a given index i (a(0)=first element):
 *    1) Find the row r such that r(r+1)/2 ≤ i < (r+1)(r+2)/2.
 *       r = ⌊(sqrt(8*i+1) – 1)/2⌋  (improved with a tiny adjustment).
 *    2) Column k = i – r(r+1)/2.
 *    3) a(i) = r² + r*k – k².
 *
 *  This is a straightforward O(1) formula.
 */

#include <math.h>
#include <stdint.h>

/* Compute integer sqrt with adjustment to guarantee floor value. */
static inline idx_t isqrt_u64(idx_t v)
{
    /* use double sqrt as initial guess; adjust in 0/1 steps */
    long double d = sqrt((long double)v);
    idx_t r = (idx_t)d;
    while ((r+1)*(r+1) <= v) ++r;
    while (r*r > v) --r;
    return r;
}

seq_t a(idx_t n)
{
    /* Step 1: determine row r.  Use sqrt(8*n+1). */
    long double d = sqrt((long double)8 * (long double)n + 1.0L);
    idx_t r = (idx_t)((d - 1.0L) / 2.0L);

    /* Ensure r satisfies the triangular bounds. */
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;

    /* Step 2: column k. */
    idx_t k = n - r * (r + 1) / 2;

    /* Step 3: compute T(r,k). */
    seq_t sr = (seq_t)r;
    seq_t sk = (seq_t)k;
    return sr * sr + sr * sk - sk * sk;
}
