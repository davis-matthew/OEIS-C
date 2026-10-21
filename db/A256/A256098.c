/*
 * A256098  Denominators for the numerators A256097.
 * https://oeis.org/A256098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = (2*floor(sqrt(n))) / gcd(n - floor(sqrt(n))^2,
 *            2*floor(sqrt(n)))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(n)) for 0 <= n <= UINT64_MAX */
static idx_t isqrt(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;
    idx_t mid;
    while (low <= high) {
        mid = low + (high - low) / 2;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n)
            return mid;
        if (sq < n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high; /* floor sqrt */
}

/* Euclidean gcd for idx_t (unsigned 64-bit) */
static idx_t gcd_uint64(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* OEIS sequence A256098: denominators for the numerators A256097 */
seq_t a(idx_t n)
{
    idx_t f = isqrt(n);                /* floor(sqrt(n)) */
    idx_t s = n - f * f;               /* n - f^2, 0 <= s <= 2f */
    idx_t two_f = 2 * f;               /* 2*f */
    idx_t g = gcd_uint64(s, two_f);    /* gcd(s, 2*f) */
    return (seq_t)(two_f / g);         /* (2*f)/gcd(s,2*f) */
}
