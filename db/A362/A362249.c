/*
 * A362249  Point number on a 4-arm square spiral of point n on the East arm
 *          scaled up by steps of that point itself.
 * https://oeis.org/A362249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   with S(n) = A340944(n) + i*A340945(n), a(n) = k where S(n)^2 =
 *            S(k)*i^arm, arm in 0..3 (inverse of the East-arm spiral map)
 * Range:     u8 n <= 11, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static __int128 A362249_isqrt(__int128 n)
{
    unsigned long long lo = 0, hi = 0xFFFFFFFFULL;
    while (lo < hi) {
        unsigned long long mid = lo + (hi - lo + 1) / 2;
        if ((__int128)mid * mid <= n) lo = mid; else hi = mid - 1;
    }
    return (__int128)lo;
}

static void A362249_S(__int128 n, __int128 *x, __int128 *y)
{
    __int128 s = A362249_isqrt(n);
    if (n > s * s + s) s++;
    __int128 r = n - s * s;
    switch ((int)(s & 3)) {
        case 0: *x = r;  *y = -s; break;
        case 1: *x = s;  *y = r;  break;
        case 2: *x = -r; *y = s;  break;
        default: *x = -s; *y = -r; break;
    }
}

seq_t a(idx_t n)
{
    __int128 x, y;
    A362249_S((__int128)n, &x, &y);
    __int128 X = x * x - y * y;
    __int128 Y = 2 * x * y;
    for (int m = 0; m < 4; m++) {
        __int128 s, r;
        /* case 0: (r, -s) */
        s = -Y; r = X;
        if (s >= 0 && (s & 3) == 0 && r > -s && r <= s)
            return (seq_t)(s * s + r);
        /* case 1: (s, r) */
        s = X; r = Y;
        if (s >= 0 && (s & 3) == 1 && r > -s && r <= s)
            return (seq_t)(s * s + r);
        /* case 2: (-r, s) */
        s = Y; r = -X;
        if (s >= 0 && (s & 3) == 2 && r > -s && r <= s)
            return (seq_t)(s * s + r);
        /* case 3: (-s, -r) */
        s = -X; r = -Y;
        if (s >= 0 && (s & 3) == 3 && r > -s && r <= s)
            return (seq_t)(s * s + r);
        /* rotate by -90 degrees: multiply by -i */
        __int128 t = Y;
        Y = -X;
        X = t;
    }
    return 0;
}
