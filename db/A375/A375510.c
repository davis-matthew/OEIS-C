/*
 * A375510  Fringe indices of Zernike polynomials.
 * https://oeis.org/A375510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   T(n,k) = (1 + (n + abs(m))/2)^2 - 2*abs(m) + [m < 0], where m =
 *            -n + 2*k. The sequence a(n) is defined by a(n) = T(r,k) with r,k
 *            chosen such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 and k = n -
 *            r(r+1)/2
 * Range:     u8 n <= 135, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Find the row index r for a given flattened index n.  */
static inline idx_t
find_row(idx_t n)
{
    /* Exponential search to bound r.  */
    idx_t low  = 0;
    idx_t high = 1;
    while (((__uint128_t)high * (high + 1)) / 2 <= n)
        high <<= 1;

    low = high >> 1;
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        __uint128_t val = (__uint128_t)mid * (mid + 1) / 2;
        if (val <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Determine row r and column k.  */
    idx_t r = find_row(n);

    /* Compute offset = r*(r+1)/2 safely.  */
    __uint128_t off128 = (__uint128_t)r * (r + 1) / 2;
    idx_t offset = (idx_t)off128;          /* offset ≤ n */
    idx_t k = n - offset;

    /* Compute m = -r + 2*k and its absolute value.  */
    int64_t m = -(int64_t)r + 2 * (int64_t)k;
    uint64_t abs_m = m >= 0 ? (uint64_t)m : (uint64_t)(-m);

    /* Evaluate T(r,k).  All intermediate arithmetic done in 128 bits. */
    __uint128_t half = ((__uint128_t)r + abs_m) / 2;   /* (r + abs(m))/2 */
    __uint128_t base = 1 + half;                     /* 1 + (r+abs(m))/2 */
    __uint128_t val  = base * base;                  /* base^2 */
    val -= (unsigned __int128)2 * abs_m;             /* -2*abs(m) */
    if (m < 0)
        val += 1;                                    /* + Iverson bracket */

    return (seq_t)val;
}
