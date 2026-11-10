/*
 * A355456  Greatest common divisor of sigma(n), A003961(n), and A276086(n).
 * https://oeis.org/A355456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(sigma(n), gcd(A003961(n), A276086(n))), where
 *            gcd(A003961(n), A276086(n)) = Product_{k>=1}
 *            prime(k+1)^min(v_{prime(k)}(n), d_k(n)), d_k = digit k of n in
 *            primorial base
 * Range:     u8 n <= 99, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9302 (2026-11-10)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t A355456_sigma(uint64_t m)
{
    seq_t result = 1;
    for (uint64_t p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            seq_t term = 1, pw = 1;
            while (m % p == 0) {
                m /= p;
                pw *= (seq_t)p;
                term += pw;
            }
            result *= term;
        }
    }
    if (m > 1) result *= ((seq_t)m + 1);
    return result;
}

seq_t a(idx_t n)
{
    uint64_t pr[32];
    int cnt = 0;
    uint64_t c;
    for (c = 2; cnt < 32; c++) {
        bool ok = true;
        for (int i = 0; i < cnt && pr[i] * pr[i] <= c; i++)
            if (c % pr[i] == 0) { ok = false; break; }
        if (ok) pr[cnt++] = c;
    }

    /* primorial-base digits d[i] with radix pr[i] */
    uint64_t d[32];
    int K = 0;
    uint64_t m = (uint64_t)n;
    while (m > 0 && K < 32) {
        d[K] = m % pr[K];
        m /= pr[K];
        K++;
    }

    seq_t g = 1;
    for (int k = 1; k < K; k++) {
        uint64_t e = 0;
        uint64_t t = (uint64_t)n;
        while (t % pr[k - 1] == 0) { t /= pr[k - 1]; e++; }
        uint64_t mn = e < d[k] ? e : d[k];
        for (uint64_t j = 0; j < mn; j++) g *= (seq_t)pr[k];
    }

    return gcd(A355456_sigma((uint64_t)n), g);
}
