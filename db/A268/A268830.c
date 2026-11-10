/*
 * A268830  Square array A(r,c): A(0,c) = c, A(r,0) = 0, A(r>=1,c>=1) =
 *          1+A(r-1,A268718(c)-1) = 1 + A(r-1, A003188(A006068(c)-1)), read by
 *          descending antidiagonals.
 * https://oeis.org/A268830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A(0,c)=c, A(r,0)=0, A(r,c)=1+A(r-1,A268718(c)-1); a(n) read by
 *            descending antidiagonals: n=d(d+1)/2+k -> r=k, c=d-k
 * Range:     u8 n <= 15769, u16 n <= 32895, u32 n <= 32895, u64 n <= 32895,
 *            u128 n <= 32895, big any n
 * Validated: standard, pass, 136 terms through n = 9094 (2026-11-07)
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt.h"

static seq_t A268830_gray(seq_t b) {
    return b ^ (b >> 1);
}

static seq_t A268830_ungray(seq_t g) {
    seq_t b = g;
    for (seq_t s = g >> 1; s; s >>= 1) b ^= s;
    return b;
}

static seq_t A268830_f(seq_t n) {
    if (n == 0) return 0;
    seq_t bi = A268830_ungray(n);
    return A268830_gray(bi - 1) + 1;
}

seq_t a(idx_t n) {
    idx_t d = (isqrt(8 * n + 1) - 1) / 2;
    while (d * (d + 1) / 2 > n) d--;
    while ((d + 1) * (d + 2) / 2 <= n) d++;
    idx_t k = n - d * (d + 1) / 2;
    idx_t r = k;
    idx_t c = d - k;
    seq_t x = (seq_t)c;
    seq_t steps = 0;
    while (r > 0) {
        if (x == 0) return steps;
        steps++;
        x = A268830_f(x) - 1;
        r--;
    }
    return steps + x;
}
