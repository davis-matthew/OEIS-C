/*
 * A368910  Partial sums of A368909.
 * https://oeis.org/A368910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(n) = a(n-1) + (A003415(sigma(A005940(n+1))) mod 2);
 *            computed by incremental cumulative sum with a parity shortcut
 *            for the arithmetic derivative
 * Range:     u8 n <= 53, u16 n <= 1013, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 103 terms through n = 8863 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static int A368910_term(idx_t k) {
    static const unsigned pr[] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101,103,107,109,113,127,131};
    unsigned ex[40];
    int i;
    for (i = 0; i < 40; i++) ex[i] = 0;
    idx_t m = k;
    int idx = 0;
    int maxi = 0;
    while (m > 0) {
        if (m & 1) {
            ex[idx]++;
            if (idx > maxi) maxi = idx;
            m = (m - 1) >> 1;
        } else {
            m >>= 1;
            idx++;
        }
    }
    seq_t s = 1;
    for (i = 0; i <= maxi; i++) {
        if (ex[i] == 0) continue;
        seq_t t = 1, pw = 1;
        unsigned j;
        for (j = 0; j < ex[i]; j++) {
            pw *= (seq_t)pr[i];
            t += pw;
        }
        s *= t;
    }
    /* parity of arithmetic derivative of s */
    if ((s & 1) == 0) {
        return ((s & 3) == 2) ? 1 : 0;
    }
    /* odd: parity of bigomega(s) */
    unsigned omega = 0;
    seq_t r = s;
    for (seq_t p = 3; p * p <= r; p += 2) {
        while (r % p == 0) {
            r /= p;
            omega++;
        }
    }
    if (r > 1) omega++;
    return (int)(omega & 1);
}

seq_t a(idx_t n) {
    static uint64_t *pre = 0;
    static idx_t have = 0, cap = 0;
    if (pre == 0) {
        cap = 1024;
        pre = (uint64_t *)malloc(cap * sizeof(uint64_t));
        pre[0] = 0;
        have = 0;
    }
    if (n > have) {
        if (n + 1 > cap) {
            while (cap < n + 1) cap *= 2;
            pre = (uint64_t *)realloc(pre, cap * sizeof(uint64_t));
        }
        while (have < n) {
            have++;
            pre[have] = pre[have - 1] + (uint64_t)A368910_term(have);
        }
    }
    return (seq_t)pre[n];
}
