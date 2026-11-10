/*
 * A276183  Genus of the quotient of the modular curve X_0(n) by the Fricke
 *          involution.
 * https://oeis.org/A276183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1 + A001617(n))/2 - r*A000003(n)/12 for n > 4, with r=4
 *            if n==3 (mod 8), r=6 if n==7 (mod 8), else r=3; a(n)=0 for n<=4
 * Range:     u8 n <= 955, u16 n <= 54321, u32 n <= 54321, u64 n <= 54321,
 *            u128 n <= 54321, big any n
 * Validated: standard, pass, 118 terms through n = 9550 (2026-11-08)
 * Helpers:   gcd_u64, totient_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_u64.h"
#include "../utils/totient_u64.h"

static seq_t A001617_a(idx_t n) {
    uint64_t m = n;
    uint64_t psi = n;
    long long nu2 = (n % 4 == 0) ? 0 : 1;
    long long nu3 = (n % 9 == 0) ? 0 : 1;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            psi = psi / p * (p + 1);
            if (p != 2) nu2 *= (p % 4 == 1) ? 2 : 0;
            if (p != 3) nu3 *= (p % 3 == 1) ? 2 : 0;
        }
    }
    if (m > 1) {
        uint64_t p = m;
        psi = psi / p * (p + 1);
        if (p != 2) nu2 *= (p % 4 == 1) ? 2 : 0;
        if (p != 3) nu3 *= (p % 3 == 1) ? 2 : 0;
    }

    uint64_t cusps = 0;
    for (uint64_t d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            uint64_t e = n / d;
            uint64_t ph = totient_u64(gcd_u64(d, e));
            cusps += ph;
            if (d != e) cusps += ph;
        }
    }

    uint64_t num = 12 + psi - 3 * (uint64_t)nu2 - 4 * (uint64_t)nu3 - 6 * cusps;
    return (seq_t)(num / 12);
}

static seq_t A000003_a(idx_t n)
{
    unsigned long long N4 = 4ULL * (unsigned long long)n;
    seq_t count = 0;
    unsigned long long A;
    for (A = 1; 3ULL * A * A <= N4; A++) {
        long long B;
        for (B = -(long long)A; B <= (long long)A; B++) {
            unsigned long long absB, num, C;
            if (B & 1) continue;
            absB = (unsigned long long)(B < 0 ? -B : B);
            num = absB * absB + N4;
            if (num % (4ULL * A) != 0) continue;
            C = num / (4ULL * A);
            if (C < A) continue;
            if (B < 0 && (absB == A || A == C)) continue;
            if (gcd_u64(gcd_u64(A, absB), C) != 1) continue;
            count++;
        }
    }
    return count;
}

seq_t a(idx_t n)
{
    if (n <= 4) return 0;
    unsigned r = 3;
    unsigned m8 = (unsigned)(n % 8);
    if (m8 == 3) r = 4;
    else if (m8 == 7) r = 6;
    seq_t g = A001617_a(n);
    seq_t h = A000003_a(n);
    seq_t pos = (seq_t)6 * ((seq_t)1 + g);
    seq_t neg = (seq_t)r * h;
    if (pos < neg) return 0;
    return (pos - neg) / 12;
}
