/*
 * A364501  a(n) = n / gcd(n, A005940(n)).
 * https://oeis.org/A364501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = n / gcd(n, A005940(n)); A005940(n) is handled via its
 *            prime exponents (no overflow)
 * Range:     u8 n <= 256, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 105 terms through n = 8907 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    int exps[80];
    int pr[80];
    int i, k, cnt;
    for (i = 0; i < 80; i++) exps[i] = 0;

    /* first 70 primes */
    cnt = 0;
    for (int c = 2; cnt < 70; c++) {
        int isp = 1;
        for (int d = 0; d < cnt && pr[d] * pr[d] <= c; d++) {
            if (c % pr[d] == 0) { isp = 0; break; }
        }
        if (isp) pr[cnt++] = c;
    }

    idx_t m = n - 1;
    i = 1;
    while (m > 0) {
        if (m & 1) {
            exps[i - 1]++;
            m >>= 1;
        } else {
            m >>= 1;
            i++;
        }
    }

    uint64_t rem = (uint64_t)n;
    for (k = 0; k < 70; k++) {
        int e = exps[k];
        uint64_t p = (uint64_t)pr[k];
        while (e > 0 && rem % p == 0) {
            rem /= p;
            e--;
        }
    }
    return (seq_t)rem;
}
