/*
 * A367194  The y-coordinate of the point where x + y = n, x and y are
 *          integers and x/y is as close as possible to Pi.
 * https://oeis.org/A367194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A367194
    a(n) is the y–coordinate of the integer point (x,y) on the line x+y=n
    for which the ratio x/y is as close as possible to π.
    It is always either floor(n/(1+π)) or ceil(n/(1+π)), and is chosen
    by comparing the two candidates using an integer fixed‑point
    approximation of 1+π. */
#include <stdint.h>

static const unsigned long long L_NUM = 4141592653589ULL;   /* (1+π)*10^12, truncated */
static const unsigned long long L_DEN = 1000000000000ULL;  /* 10^12 */

seq_t a(idx_t n)
{
    /* n = 0 is outside the offset – return 0 */
    if (n == 0) return 0;

    unsigned long long nn = (unsigned long long)n;

    /* floor(n/(1+π)) using the fixed‑point quotient */
    unsigned long long y0 = (unsigned long long)(((__uint128_t)nn * L_DEN) / L_NUM);

    /* y must be at least 1 */
    if (y0 < 1) y0 = 1;

    unsigned long long y1 = y0;
    unsigned long long y2 = y1 + 1;

    /* If the upper candidate exceeds n, only y1 is valid */
    if (y2 > nn) return (seq_t)y1;

    /* Compute the midpoint between r(y1)=n/y1 and r(y1+1)=n/(y1+1) */
    unsigned long long m_num = nn * (2 * y1 + 1);     /* numerator of the midpoint   */
    unsigned long long m_den = 2 * y1 * (y1 + 1);     /* denominator of the midpoint */

    /* Compare L with the midpoint:  L >= m  iff  L_num/L_den >= m_num/m_den  */
    __uint128_t left  = (__uint128_t)L_NUM * m_den;
    __uint128_t right = (__uint128_t)m_num * L_DEN;

    return (left >= right) ? (seq_t)y1 : (seq_t)y2;
}
