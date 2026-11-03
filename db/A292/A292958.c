/*
 * A292958  Rectangular array by antidiagonals: T(n,m) = rank of n*(r+m) when
 *          all the numbers k*(r+h), where r = sqrt(5), k>=1, h>=0, are
 *          jointly ranked.
 * https://oeis.org/A292958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is the rank of the element corresponding to the
 *            (i,j)antidiagonal coordinates of n in the array T(i,j) defined
 *            by OEIS A292958: T(i,j) = rank of i*(sqrt(5)+j) among all
 *            k*(sqrt(5)+h) with k>=1, h>=0. The formula for the rank is:
 *            rank(i,j) = 1 + Σ_{k=1}^{kmax} (floor((i*j + (i-k)*sqrt(5)) /
 *            k...
 * Range:     u8 n <= 126, u16 n <= 1830, u32 n <= 1830, u64 n <= 1830,
 *            u128 n <= 1830, big any n
 * Validated: standard, pass, 81 terms through n = 1701 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compare L < A + B*sqrt(5) without floating point.
   For B >= 0: return (L < A) or ((L-A)^2 < 5*B^2).
   For B < 0 : return (L >= A) ? false : ((A-L)^2 > 5*(-B)^2).                           */
static inline bool less_than_sqrt5(idx_t L, idx_t A, int64_t B)
{
    if (B == 0) {
        return L < A;
    } else if (B > 0) {
        if (L < A) {
            return true;
        }
        __int128 d  = (__int128)L - (__int128)A;            /* L-A >= 0 */
        __int128 rhs = (__int128)5 * (__int128)B * (__int128)B;
        return d * d < rhs;
    } else { /* B < 0 */
        if (L >= A) {
            return false;
        }
        __int128 d  = (__int128)A - (__int128)L;            /* A-L > 0 */
        __int128 rhs = (__int128)5 * (__int128)(-B) * (__int128)(-B);
        return d * d > rhs;
    }
}

/* Floor division by sqrt(5) using binary search.
   Returns floor(x / sqrt(5)) for non‑negative x.                                            */
static inline uint64_t floor_div_sqrt5(uint64_t x)
{
    uint64_t lo = 0, hi = x;
    while (lo <= hi) {
        uint64_t mid = lo + (hi - lo) / 2;
        /* condition: mid <= x / sqrt(5)  <=>  x >= mid*sqrt(5)   */
        if (less_than_sqrt5(x, 0, (int64_t)mid)) {    /* mid*sqrt5 > x   */
            if (mid == 0) { /* cannot get smaller */
                break;
            }
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }
    return hi;
}

/* Rank of i*(sqrt(5)+j) among all k*(sqrt(5)+h) with k>=1,h>=0. */
static inline seq_t rank_n_m(idx_t i, idx_t j)
{
    uint64_t prod     = i * j;                          /* i*j  */
    uint64_t kmax     = i + floor_div_sqrt5(prod);     /* floor(i + i*j/sqrt5) */

    seq_t sum = 0;
    for (uint64_t k = 1; k <= kmax; ++k) {
        uint64_t a = i * j;             /* integer part of D */
        int64_t  b = (int64_t)i - (int64_t)k; /* coefficient of sqrt(5) in D */

        /* Check if D = a + b*sqrt5 is positive. */
        if (!less_than_sqrt5(0, a, b))
            continue;                  /* no h-values for this k */

        /* Upper bound for h.  A conservative bound is (a + 5*i)/k + 5. */
        uint64_t hi = (a + 5 * i) / k + 5;
        uint64_t lo = 0;
        uint64_t best = 0;
        while (lo <= hi) {
            uint64_t mid = lo + (hi - lo) / 2;
            uint64_t left = k * mid;        /* k*h  */
            if (less_than_sqrt5(left, a, b)) {
                best = mid;
                lo   = mid + 1;
            } else {
                if (mid == 0) break;
                hi = mid - 1;
            }
        }
        sum += (seq_t)(best + 1);          /* number of h-values for this k */
    }
    return sum + (seq_t)1;                  /* include the element itself */
}

/* Convert the linear index n (1‑based) to matrix coordinates (i,j) (i>=1,j>=0)
   following the antidiagonal reading.                                                       */
static inline void antidiagonal_coords(idx_t n, idx_t *i, idx_t *j)
{
    idx_t lo = 1, hi = 1;
    /* grow hi until triangular(hi) >= n */
    while ((__int128)hi * (hi + 1) / 2 < n) {
        hi <<= 1;
    }
    /* binary search for minimal d with d*(d+1)/2 >= n */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t d = lo;
    __int128 prev = (__int128)d * (d - 1) / 2;
    idx_t offset = n - (idx_t)prev - 1;
    *i = offset + 1;
    *j = d - offset - 1;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset safety */
    idx_t i, j;
    antidiagonal_coords(n, &i, &j);
    return rank_n_m(i, j);
}
