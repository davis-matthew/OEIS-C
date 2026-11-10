/*
 * A290705  Theta series of triamond.
 * https://oeis.org/A290705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(2k) = A004013(k), a(2k+1) = 3*A045828(k) (interleaved);
 *            A004013(k) = #{(x,y,z) integers all even or all odd, x^2+y^2+z^2
 *            = k}, A045828(k) = #{(t1,t2,t3) triangular numbers: t1+t2+2*t3 =
 *            k}
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 72 (2026-11-08)
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_u64.h"

static unsigned long long A290705_bcc(long long N) {
    unsigned long long cnt = 0;
    long long s = (long long)isqrt_u64((unsigned long long)N);
    long long x, y;
    for (x = -s; x <= s; x++) {
        long long rx = N - x * x;
        if (rx < 0) continue;
        for (y = -s; y <= s; y++) {
            long long r = rx - y * y;
            long long z;
            if (r < 0) continue;
            if (((x - y) & 1) != 0) continue;
            z = (long long)isqrt_u64((unsigned long long)r);
            if (z * z != r) continue;
            if (((z - x) & 1) != 0) continue;
            if (z == 0) cnt += 1;
            else cnt += 2;
        }
    }
    return cnt;
}

static int A290705_is_tri(long long v) {
    unsigned long long d = 8ULL * (unsigned long long)v + 1ULL;
    unsigned long long r = isqrt_u64(d);
    return r * r == d;
}

static unsigned long long A290705_tri3(long long m) {
    unsigned long long total = 0;
    long long c;
    for (c = 0; c * (c + 1) <= m; c++) {
        long long rem = m - c * (c + 1);
        long long i;
        for (i = 0; i * (i + 1) / 2 <= rem; i++) {
            if (A290705_is_tri(rem - i * (i + 1) / 2)) total++;
        }
    }
    return total;
}

seq_t a(idx_t n) {
    long long k = (long long)(n >> 1);
    if ((n & 1) == 0) return (seq_t)A290705_bcc(k);
    return (seq_t)3 * (seq_t)A290705_tri3(k);
}
