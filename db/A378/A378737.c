/*
 * A378737  Abundant numbers k for which A378665(k) > A378664(k).
 * https://oeis.org/A378737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th abundant number k (A005101) with A378665(k) >
 *            A378664(k); found by scanning k and enumerating divisors
 * Range:     u8 n <= 6, u16 n <= 1938, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 74 terms through n = 8602 (2026-11-10)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

static void A378737_rec(const uint64_t *pr, const uint64_t *nx, const int *ex,
                        int cnt, int idx, uint64_t d, uint64_t sg, uint64_t a3,
                        uint64_t *best1, uint64_t *best2)
{
    if (idx == cnt) {
        if (sg <= 2 * d) {
            if (d > *best1) *best1 = d;
            if (2 * d < a3 && d > *best2) *best2 = d;
        }
        return;
    }
    uint64_t p = pr[idx], q = nx[idx];
    uint64_t pw = 1, qw = 1, s = 1;
    for (int e = 0; e <= ex[idx]; ++e) {
        A378737_rec(pr, nx, ex, cnt, idx + 1, d * pw, sg * s, a3 * qw, best1, best2);
        pw *= p;
        qw *= q;
        s += pw;
    }
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    uint64_t m = 11;
    for (;;) {
        m++;
        uint64_t pr[20], nx[20];
        int ex[20];
        int cnt = 0;
        uint64_t t = m, sigma = 1;
        for (uint64_t p = 2; p * p <= t; p++) {
            if (t % p == 0) {
                int e = 0;
                uint64_t pw = 1, s = 1;
                while (t % p == 0) {
                    t /= p;
                    e++;
                    pw *= p;
                    s += pw;
                }
                pr[cnt] = p; ex[cnt] = e; cnt++;
                sigma *= s;
            }
        }
        if (t > 1) {
            pr[cnt] = t; ex[cnt] = 1; cnt++;
            sigma *= (t + 1);
        }
        if (sigma <= 2 * m) continue;
        for (int i = 0; i < cnt; i++) nx[i] = next_prime(pr[i]);
        uint64_t b1 = 1, b2 = 1;
        A378737_rec(pr, nx, ex, cnt, 0, 1, 1, 1, &b1, &b2);
        if (b1 > b2) {
            count++;
            if (count == n) return (seq_t)m;
        }
    }
}
