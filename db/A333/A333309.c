/*
 * A333309  Numbers that are not the sum of two distinct terms of A003622.
 * https://oeis.org/A333309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th positive integer that is not t(i)+t(j) with i<j,
 *            where t(k) = floor(k*phi)+k-1 (A003622); computed by sieve
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include "../utils/mem.h"

static long long A333309_isqrt(long long x) {
    long long r = (long long)sqrt((double)x);
    while (r * r > x) r--;
    while ((r + 1) * (r + 1) <= x) r++;
    return r;
}

/* A003622 term: floor(k*phi) + k - 1 */
static long long A333309_t(long long k) {
    long long f = (k + A333309_isqrt(5 * k * k)) / 2;
    return f + k - 1;
}

seq_t a(idx_t n) {
    long long N = (long long)n;
    long long L = 3 * N + 10;
    for (;;) {
        long long cnt = 0, i, j;
        long long *t = (long long *)malloc(sizeof(long long) * (size_t)(L + 2));
        unsigned char *mark = (unsigned char *)calloc((size_t)(L + 2), 1);
        long long m = 0;
        for (i = 1;; i++) {
            long long v = A333309_t(i);
            if (v > L) break;
            t[m++] = v;
        }
        for (i = 0; i < m; i++)
            for (j = i + 1; j < m; j++) {
                long long s = t[i] + t[j];
                if (s > L) break;
                mark[s] = 1;
            }
        for (i = 1; i <= L; i++) {
            if (!mark[i]) {
                cnt++;
                if (cnt == N) {
                    free(t);
                    free(mark);
                    return (seq_t)i;
                }
            }
        }
        free(t);
        free(mark);
        L *= 2;
    }
}
