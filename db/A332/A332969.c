/*
 * A332969  a(n) = [x^n] (Sum_{j>=0} A002193(1-j) * x^j)^2.
 * https://oeis.org/A332969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Sum_{j=0..n} c(j)*c(n-j), where c(j) = A002193(j+1) is
 *            the (j+1)-th decimal digit of sqrt(2) (c(0)=1, c(1)=4, ...);
 *            this is [x^n] of (Sum_{j>=0} c(j) x^j)^2
 * Range:     u8 n <= 20, u16 n <= 3021, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9243 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static unsigned char *A002193_dig = NULL;
static idx_t A002193_cap = 0;

static void A002193_compute(idx_t D) {
    size_t L = 1 + (size_t)((D + 8) / 9) + 4;
    uint32_t *t = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *s = (uint32_t *)calloc(L, sizeof(uint32_t));
    const uint64_t B = 1000000000ULL;
    size_t i;
    uint64_t k;
    t[0] = 1;
    s[0] = 1;
    for (k = 1;; k++) {
        uint64_t carry = 0, rem = 0, m = 2 * k - 1, d = 100 * k;
        int nz = 0;
        for (i = L; i-- > 0;) {
            uint64_t v = (uint64_t)t[i] * m + carry;
            t[i] = (uint32_t)(v % B);
            carry = v / B;
        }
        for (i = 0; i < L; i++) {
            uint64_t v = rem * B + t[i];
            t[i] = (uint32_t)(v / d);
            rem = v % d;
            if (t[i]) nz = 1;
        }
        if (!nz) break;
        carry = 0;
        for (i = L; i-- > 0;) {
            uint64_t v = (uint64_t)s[i] + t[i] + carry;
            s[i] = (uint32_t)(v % B);
            carry = v / B;
        }
    }
    {
        uint64_t carry = 0, rem = 0;
        for (i = L; i-- > 0;) {
            uint64_t v = (uint64_t)s[i] * 7 + carry;
            s[i] = (uint32_t)(v % B);
            carry = v / B;
        }
        for (i = 0; i < L; i++) {
            uint64_t v = rem * B + s[i];
            s[i] = (uint32_t)(v / 5);
            rem = v % 5;
        }
    }
    free(A002193_dig);
    A002193_dig = (unsigned char *)malloc((size_t)D + 1);
    A002193_dig[0] = 0;
    A002193_dig[1] = (unsigned char)(s[0] % 10);
    {
        idx_t n;
        for (n = 2; n <= D; n++) {
            idx_t p = n - 2;
            uint32_t limb = s[1 + p / 9];
            int pos = (int)(p % 9);
            int j;
            for (j = 0; j < 8 - pos; j++) limb /= 10;
            A002193_dig[n] = (unsigned char)(limb % 10);
        }
    }
    A002193_cap = D;
    free(t);
    free(s);
}

static void A002193_ensure(idx_t n) {
    if (n > A002193_cap) {
        idx_t D = n * 2;
        if (D < 1024) D = 1024;
        A002193_compute(D);
    }
}

seq_t a(idx_t n) {
    unsigned long long sum = 0;
    idx_t j;
    A002193_ensure(n + 1);
    for (j = 0; j <= n; j++) {
        sum += (unsigned long long)A002193_dig[j + 1] * (unsigned long long)A002193_dig[n - j + 1];
    }
    return (seq_t)sum;
}
