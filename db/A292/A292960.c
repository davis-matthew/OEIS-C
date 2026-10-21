/*
 * A292960  Rectangular array by antidiagonals: T(n,m) = rank of n*(r+m) when
 *          all the numbers k*(r+h), where r = ((1+sqrt(5))/2)^2, k>=1, h>=0,
 *          are jointly ranked.
 * https://oeis.org/A292960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 126, u16 n <= 1830, u32 n <= 1830, u64 n <= 1830,
 *            u128 n <= 1830, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A292960  
    a(n) is obtained by reading the Beatty‑array T(n,m) along anti‑diagonals.  
    The rank function is defined by  
        T(n,m) = Σ_{k=1}^{⌊n + m*n/r⌋} ⌊1 - r + n*(r+m)/k⌋ ,  
    where r = ((1+√5)/2)².  The code below implements this definition
    exactly using only integer arithmetic (the irrational √5 is
    represented by a high‑precision integer multiple).  This avoids the
    need for floating point operations.  */

#include <stdint.h>

/* Scale factor for the rational representation of √5:
   √5 ≈ 2.23606797749979 → 2.236... * 10¹² ≈ 2236067977499.      */
static const __int128 SCALE     = (__int128)1000000000000ULL;   /* 1e12   */
static const __int128 SQRT5_NUM = (__int128)2236067977499ULL;  /* √5 * SCALE */

/* floor division of a signed 128‑bit numerator by a positive 128‑bit denom */
static inline int64_t floor_div128(__int128 num, __int128 den)
{
    if (num >= 0) return (int64_t)(num / den);
    /* num < 0 : compute floor towards -∞ */
    __int128 abs_num = -num;
    __int128 q = abs_num / den;
    __int128 r = abs_num % den;
    return -(int64_t)(r == 0 ? q : q + 1);
}

/* term k of the summation for T(n,m):
      ⌊1 - r + n*(r+m)/k⌋
   Derived as floor( (a + b·√5) / d ) with integer a,b,d. */
static inline int64_t term_floor(int64_t n, int64_t m, int64_t k)
{
    int64_t a = 2*n*m + 3*n - k;   /* integer part of numerator   */
    int64_t b = n - k;             /* coefficient of √5 in numerator */
    int64_t d = 2*k;               /* denominator                */

    __int128 num = (__int128)a * SCALE + (__int128)b * SQRT5_NUM;
    __int128 den = (__int128)d * SCALE;

    return floor_div128(num, den);
}

/* compute T(n,m) by the summation until the term becomes negative. */
static inline seq_t compute_T(idx_t r_n, idx_t c_m)
{
    int64_t n = (int64_t)r_n;
    int64_t m = (int64_t)c_m;
    int64_t sum = 0;

    for (idx_t k = 1;; ++k) {
        int64_t t = term_floor(n, m, (int64_t)k);
        if (t < 0) break;          /* further terms are non‑positive   */
        sum += t;
    }
    return (seq_t)sum;
}

/*  Main sequence function.  The index n (1‑based) is mapped to the
    anti‑diagonal coordinates (row ≥ 1, col ≥ 0) and T(row,col) is
    evaluated.  */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n = 0, return 0 */

    /* find diagonal number s such that 1 ≤ n ≤ s*(s+1)/2   */
    idx_t s = 1;
    while (s * (s + 1) / 2 < n) ++s;

    idx_t prev = (s - 1) * s / 2;          /* elements before diag s */
    idx_t pos  = n - prev - 1;             /* 0‑based position in diag s */
    idx_t row  = pos + 1;                  /* rows start at 1          */
    idx_t col  = s - row;                  /* columns start at 0       */

    return compute_T(row, col);
}
