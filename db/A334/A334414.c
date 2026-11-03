/*
 * A334414  First differences of A334415.
 * https://oeis.org/A334414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round((n+1)/phi^2) - round(n/phi^2)
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Validated: partial, pass, 89 terms through n = 88 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          a(n) = round((n+1)/phi^2) - round(n/phi^2)
   where phi^2 = (3+sqrt(5))/2 */
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdbool.h>

/* Helper: decide whether a + b*sqrt(5) < 0 using only integer arithmetic.
   a,b are signed 128-bit integers.  The result is true if the value is strictly
   negative, false otherwise. */
static inline bool less_zero(__int128 a, __int128 b)
{
    if (b == 0) {
        return a < 0;
    }
    if (b > 0) {
        /* a + b*sqrt5 < 0  <=>  a < -b*sqrt5  <=>  a < 0  and  (-a)^2 > 5*b^2  */
        if (a >= 0)
            return false;
        __int128 aa = -a;          /* aa > 0 */
        return ( (__int128)5 * b * b < aa * aa );
    } else { /* b < 0 */
        /* a + b*sqrt5 < 0  <=>  a < -b*sqrt5  <=>  a <= 0  or  (5*(-b)^2 > a^2) */
        __int128 bb = -b;          /* bb > 0 */
        if (a <= 0)
            return true;
        return ( (__int128)5 * bb * bb > a * a );
    }
}

/* Compute floor[(p + q*sqrt5)/2] where p,q are 128-bit integers.
   This is used to evaluate round(k/phi^2) = floor[(3k+1 - k*sqrt5)/2]. */
static inline seq_t floor_of_sqrt5_ratio(idx_t k)
{
    __int128 p = (__int128)3 * k + 1;  /* 3k + 1 */
    __int128 q = -(__int128)k;        /* -k */
    /* Double approximation for the initial value.  sqrt5 is constant  */
    static const double sqrt5_val = 2.2360679774997898;
    double approx = (((double)p) + ((double)q) * sqrt5_val) * 0.5;
    long long m = (long long)floor(approx);

    /* Adjust up if we are too low.  We need smallest integer m with
       2*(m+1) > p + q*sqrt5, i.e. 2*(m+1) > p + q*sqrt5  */
    while (!less_zero(p - (__int128)2 * (m + 1), q))
        m++;

    /* Adjust down if we are too high.  We need 2*m <= p + q*sqrt5  */
    while (less_zero(p - (__int128)2 * m, q))
        m--;

    return (seq_t)m;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* a(n) = round((n+1)/phi^2) - round(n/phi^2) = S(n+1)-S(n) */
    seq_t s_n  = floor_of_sqrt5_ratio(n);
    seq_t s_np = floor_of_sqrt5_ratio(n + 1);
    return (s_np > s_n) ? 1 : 0;
}
