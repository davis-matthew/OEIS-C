/*
 * A249099  Position of 3*n^6 in the ordered union of {h^6, h >=1} and {3*k^6,
 *          k >=1}.
 * https://oeis.org/A249099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((1+3^(1/6)) * n)
 * Range:     u8 n <= 116, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9429 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* integer sixth root: floor( v^(1/6) ) for v >= 0, using binary search */
static inline __int128 floor_sixth_root(__int128 v)
{
    __int128 lo = 0;
    /* an upper bound: (3^(1/6)) * n < 2 * n for all n >= 1, so 2*n+1 is safe */
    __int128 hi = v > 0 ? ((v * 6) / 3) : 1;   /* approximate hi needed only for v>0 */
    /* For the specific use we call the function with v = 3*n^6, thus hi = 2*n+1 */
    /* We set hi conservatively to 2*n+1. This requires the caller to provide a safe hi. */
    return lo; /* placeholder, actual hi must be set by caller */
}

/* integer sixth power using __int128 to avoid overflow for n up to at least 10⁴ */
static inline __int128 pow6_int128(__int128 x)
{
    __int128 x2 = x * x;      /* x^2  */
    __int128 x3 = x2 * x;     /* x^3  */
    __int128 x6 = x3 * x3;    /* x^6  */
    return x6;
}

/* Compute the Beatty sequence for 3*n^6 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* defined only for n >= 1, but guard anyway */

    /* use 64-bit unsigned for n (idx_t is uint64_t) */
    unsigned long long un = (unsigned long long)n;   /* safe cast */

    /* compute t = 3 * n^6 in __int128 to avoid overflow */
    __int128 n128 = (__int128)un;
    __int128 n6 = pow6_int128(n128);       /* n^6 */
    __int128 t = 3 * n6;                   /* 3 * n^6 */

    /* binary search for floor(t^(1/6)).  The integer sixth root is < 2*n+1. */
    __int128 lo = 0;
    __int128 hi = n128 * 2 + 1;            /* safe upper bound: 3^(1/6) < 2 */
    while (lo < hi) {
        __int128 mid = (lo + hi + 1) >> 1;  /* ceil of average to avoid infinite loop */
        __int128 mid6 = pow6_int128(mid);
        if (mid6 <= t)
            lo = mid;
        else
            hi = mid - 1;
    }
    __int128 root = lo;                    /* floor(n * 3^(1/6)) */

    /* a(n) = n + floor(n * 3^(1/6)) */
    return (seq_t)un + (seq_t)root;
}
