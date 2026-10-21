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
 * Helpers:   floor_sixth_root, pow6_int128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sixth_root.h"
#include "../utils/pow6_int128.h"

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
