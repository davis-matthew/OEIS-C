/*
 * A324179  Number of distinct values A297167 obtains over divisors > 1 of n,
 *          minus number of prime factors of n counted with multiplicity: a(n)
 *          = A324190(n) - A001222(n).
 * https://oeis.org/A324179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A324190(n) - A001222(n), where A324190(n) = number of
 *            distinct values of A297167(d) over divisors d>1 of n; A297167(d)
 *            = Omega(d)-omega(d)+pi(lpf(d))-1
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9018 (2026-11-08)
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/prime_pi.h"

static int A324179_cmp(const void *x, const void *y) {
    uint64_t a = *(const uint64_t *)x, b = *(const uint64_t *)y;
    return (a > b) - (a < b);
}

seq_t a(idx_t n) {
    if (n <= 1) return 0;
    uint64_t ps[24];
    int es[24];
    int k = 0;
    idx_t t = n;
    uint64_t total_omega = 0;
    for (idx_t p = 2; p <= t / p; p += (p == 2 ? 1 : 2)) {
        if (t % p == 0) {
            int e = 0;
            while (t % p == 0) { t /= p; e++; }
            ps[k] = (uint64_t)p; es[k] = e; k++;
            total_omega += (uint64_t)e;
        }
    }
    if (t > 1) { ps[k] = (uint64_t)t; es[k] = 1; k++; total_omega++; }

    uint64_t pis[24];
    for (int i = 0; i < k; i++) pis[i] = (uint64_t)(unsigned long long)prime_pi((idx_t)ps[i]);

    uint64_t nd = 1;
    for (int i = 0; i < k; i++) nd *= (uint64_t)(es[i] + 1);

    uint64_t *vals = (uint64_t *)malloc(nd * sizeof(uint64_t));
    uint64_t cnt = 0;
    int f[24];
    for (int i = 0; i < k; i++) f[i] = 0;
    for (;;) {
        /* odometer increment */
        int i = 0;
        while (i < k && f[i] == es[i]) { f[i] = 0; i++; }
        if (i == k) break;
        f[i]++;
        uint64_t om = 0, w = 0;
        int top = -1;
        for (int j = 0; j < k; j++) {
            if (f[j] > 0) { om += (uint64_t)f[j]; w++; top = j; }
        }
        vals[cnt++] = om - w + pis[top] - 1;
    }
    qsort(vals, (size_t)cnt, sizeof(uint64_t), A324179_cmp);
    uint64_t distinct = 0;
    for (uint64_t i = 0; i < cnt; i++) {
        if (i == 0 || vals[i] != vals[i - 1]) distinct++;
    }
    free(vals);
    return (seq_t)(distinct - total_omega);
}
