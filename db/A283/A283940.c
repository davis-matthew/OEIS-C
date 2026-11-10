/*
 * A283940  Interspersion of the signature sequence of sqrt(3).
 * https://oeis.org/A283940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   A283940 is the array whose row r lists the k with A007337(k)=r,
 *            read by antidiagonals (T(1,d), T(2,d-1), ..., T(d,1)). The code
 *            generates A007337 by an exact-integer band search and counts
 *            occurrences of r
 * Range:     u8 n <= 153, u16 n <= 1830, u32 n <= 1830, u64 n <= 1830,
 *            u128 n <= 1830, big any n
 * Validated: standard, pass, 82 terms through n = 1716 (2026-11-08)
 * Helpers:   isqrt_i64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/isqrt_i64.h"

static long long A283940_fl(long long j) {
    return isqrt_i64(3 * j * j);
}

/* true iff frac(a*sqrt3) < frac(b*sqrt3), a != b */
static int A283940_less(long long a, long long b) {
    long long d = a - b;
    long long e = A283940_fl(a) - A283940_fl(b);
    if (d > 0) {
        if (e <= 0) return 0;
        return 3 * d * d < e * e;
    } else {
        if (e >= 0) return 1;
        return 3 * d * d > e * e;
    }
}

seq_t a(idx_t n) {
    unsigned long long d = 1, nn = (unsigned long long)n;
    unsigned long long r, c;
    while (d * (d + 1) / 2 < nn) d++;
    r = nn - d * (d - 1) / 2;
    c = d + 1 - r;

    long long cap = 64, J = 0;
    long long *ord = (long long *)malloc((size_t)cap * sizeof(long long));
    unsigned long long count = 0, cnt = 0;
    long long k;
    for (k = 1;; k++) {
        long long t;
        while (3 * (J + 1) * (J + 1) < k * k) {
            long long j = J + 1, p;
            if (J + 1 > cap) {
                cap *= 2;
                ord = (long long *)realloc(ord, (size_t)cap * sizeof(long long));
            }
            p = J;
            while (p > 0 && A283940_less(j, ord[p - 1])) {
                ord[p] = ord[p - 1];
                p--;
            }
            ord[p] = j;
            J++;
        }
        for (t = 0; t < J; t++) {
            long long i = k - A283940_fl(ord[t]);
            count++;
            if ((unsigned long long)i == r) {
                cnt++;
                if (cnt == c) {
                    free(ord);
                    return (seq_t)count;
                }
            }
        }
    }
}
