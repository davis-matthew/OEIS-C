/*
 * A291957  Triangle read by rows: T(n,k) = T(n-k,k-1) - T(n-k,k) + T(n-k,k+1)
 *          with T(0,0) = 1 for 0 <= k <= A003056(n).
 * https://oeis.org/A291957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf
 * Formula:   T(n,k) = T(n-k,k-1) - T(n-k,k) + T(n-k,k+1), T(0,0)=1, T(n,0)=0
 *            for n>0, T(n,k)=0 out of range, 0<=k<=A003056(n); a(n) = n-th
 *            entry of rows read in order
 * Range:     i8 n <= 767, i16 n <= 3841, i32 n <= 9981, i64 n <= 9981,
 *            i128 n <= 9981, big any n
 * Validated: standard, pass, 98 terms through n = 9685 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

static unsigned long long A003056_rowmax(unsigned long long r)
{
    unsigned long long lo = 0, hi = 1ULL << 32;
    while (hi - lo > 1) {
        unsigned long long mid = lo + (hi - lo) / 2;
        if (mid * (mid + 1) / 2 <= r) lo = mid; else hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    unsigned long long r = 0, rem = (unsigned long long)n;
    while (1) {
        unsigned long long len = A003056_rowmax(r) + 1;
        if (rem < len) break;
        rem -= len;
        r++;
    }
    unsigned long long K = rem;
    unsigned long long W = A003056_rowmax(r) + 3;
    seq_t *T = mem_new(seq_t, (size_t)((r + 1) * W));
    T[0] = 1;
    for (unsigned long long m = 1; m <= r; m++) {
        unsigned long long s = A003056_rowmax(m);
        for (unsigned long long k = 1; k <= s; k++) {
            seq_t *p = T + (size_t)((m - k) * W);
            seq_t v = p[k - 1] - p[k] + p[k + 1];
            T[(size_t)(m * W + k)] = v;
        }
    }
    seq_t res = T[(size_t)(r * W + K)];
    mem_free(T);
    return res;
}
