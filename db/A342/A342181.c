/*
 * A342181  Product of first n Robbins numbers.
 * https://oeis.org/A342181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} A005130(k), A005130(k) =
 *            Product_{j=0..k-1} (3j+1)!/(k+j)!, computed by exact
 *            prime-exponent counting
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 11 (2026-11-10)
 * Helpers:   ipow_i64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i64.h"

seq_t a(idx_t n) {
    if (n <= 1) return 1;
    long long nn = (long long)n;
    long long N = 3 * nn - 2;
    long long *spf = (long long *)calloc((size_t)(N + 1), sizeof(long long));
    long long *mult = (long long *)calloc((size_t)(N + 1), sizeof(long long));
    long long *ex = (long long *)calloc((size_t)(N + 1), sizeof(long long));
    for (long long i = 2; i <= N; i++) {
        if (spf[i] == 0) {
            for (long long j = i; j <= N; j += i)
                if (spf[j] == 0) spf[j] = i;
        }
    }
    for (long long k = 1; k <= nn; k++) {
        long long top = 3 * k - 2;
        for (long long m = 2; m <= top; m++) {
            long long lo1 = (m + 1) / 3;
            long long c1 = k - lo1;
            if (c1 < 0) c1 = 0;
            long long lo2 = m - k;
            if (lo2 < 0) lo2 = 0;
            long long c2 = k - lo2;
            if (c2 < 0) c2 = 0;
            mult[m] += c1 - c2;
        }
    }
    for (long long m = 2; m <= N; m++) {
        long long e = mult[m];
        if (e == 0) continue;
        long long x = m;
        while (x > 1) {
            long long p = spf[x];
            while (x % p == 0) {
                x /= p;
                ex[p] += e;
            }
        }
    }
    seq_t r = 1;
    for (long long p = 2; p <= N; p++) {
        if (ex[p] > 0) r *= ipow_i64((seq_t)p, ex[p]);
    }
    free(spf);
    free(mult);
    free(ex);
    return r;
}
