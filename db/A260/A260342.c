/*
 * A260342  A002175 omitting the terms where A002107 is zero.
 * https://oeis.org/A260342
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A002175(m) where m is the n-th (from 0) index with
 *            A002107(m) != 0; A002107 = coefficients of Product_{k>=1}
 *            (1-x^k)^2, A002175(m) = (divisors of 12m+1 that are 1 mod 4) -
 *            (those 3 mod 4)
 * Range:     u8 n <= 29, u16 n <= 29, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Validated: partial, pass, 30 terms through n = 29 (2026-11-07)
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/isqrt_u64.h"

static int a260342_eta(uint64_t m) {
    uint64_t t = 24 * m + 1;
    uint64_t s = isqrt_u64(t);
    if (s * s != t) return 0;
    uint64_t j = (s % 6 == 5) ? (s + 1) / 6 : (s - 1) / 6;
    return (j & 1) ? -1 : 1;
}

/* coefficient of x^m in Product (1-x^k)^2 */
static long long a260342_c2107(uint64_t m) {
    long long sum = 0;
    for (uint64_t k = 0;; k++) {
        uint64_t g1 = (k == 0) ? 0 : k * (3 * k - 1) / 2;
        if (g1 > m) break;
        int sg = (k & 1) ? -1 : 1;
        sum += sg * a260342_eta(m - g1);
        if (k > 0) {
            uint64_t g2 = k * (3 * k + 1) / 2;
            if (g2 <= m) sum += sg * a260342_eta(m - g2);
        }
    }
    return sum;
}

static long long a260342_c2175(uint64_t n) {
    uint64_t m = n * 12 + 1;
    long long diff = 0;
    for (uint64_t d = 1; d * d <= m; d++) {
        if (m % d == 0) {
            uint64_t d2 = m / d;
            if (d % 4 == 1) diff++; else if (d % 4 == 3) diff--;
            if (d2 != d) {
                if (d2 % 4 == 1) diff++; else if (d2 % 4 == 3) diff--;
            }
        }
    }
    return diff;
}

static uint64_t *a260342_pos = 0;
static size_t a260342_cnt = 0, a260342_cap = 0;
static uint64_t a260342_next = 0;

seq_t a(idx_t n) {
    while (a260342_cnt <= (size_t)n) {
        uint64_t m = a260342_next++;
        if (a260342_c2107(m) != 0) {
            if (a260342_cnt == a260342_cap) {
                a260342_cap = a260342_cap ? a260342_cap * 2 : 1024;
                a260342_pos = (uint64_t *)realloc(a260342_pos, a260342_cap * sizeof(uint64_t));
            }
            a260342_pos[a260342_cnt++] = m;
        }
    }
    return (seq_t)a260342_c2175(a260342_pos[n]);
}
