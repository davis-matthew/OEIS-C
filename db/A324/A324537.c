/*
 * A324537  a(n) = A003557(k), where k = Product_{d|n, d>2} prime(A297167(d)).
 * https://oeis.org/A324537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003557(k), k = Product_{d|n, d>2} prime(A297167(d));
 *            computed via prime exponents: a(n) = Product p^(e_p-1) over
 *            primes p occurring with multiplicity e_p>=1
 * Range:     u8 n <= 59, u16 n <= 119, u32 n <= 359, u64 n <= 1259,
 *            u128 n <= 5039, big any n
 * Validated: standard, pass, 117 terms through n = 8105 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool A324537_isp(uint64_t x) {
    if (x < 2) return false;
    for (uint64_t i = 2; i * i <= x; i++)
        if (x % i == 0) return false;
    return true;
}

static uint64_t A324537_v(uint64_t d) {
    uint64_t big = 0, om = 0, lpf = 1, t = d;
    for (uint64_t p = 2; p * p <= t; p++) {
        if (t % p == 0) {
            om++;
            lpf = p;
            while (t % p == 0) { t /= p; big++; }
        }
    }
    if (t > 1) { om++; big++; lpf = t; }
    uint64_t pi = 0;
    for (uint64_t x = 2; x <= lpf; x++)
        if (A324537_isp(x)) pi++;
    return (big - om) + pi - 1;
}

static uint64_t A324537_nth(uint64_t m) {
    uint64_t c = 0, x = 1;
    while (c < m) {
        x++;
        if (A324537_isp(x)) c++;
    }
    return x;
}

static int A324537_cmp(const void *a, const void *b) {
    uint64_t x = *(const uint64_t *)a, y = *(const uint64_t *)b;
    return (x > y) - (x < y);
}

seq_t a(idx_t n) {
    uint64_t N = (uint64_t)n;
    size_t cap = 256, cnt = 0;
    uint64_t *vals = (uint64_t *)malloc(cap * sizeof(uint64_t));
    for (uint64_t i = 1; i * i <= N; i++) {
        if (N % i != 0) continue;
        uint64_t ds[2];
        int k = 0;
        ds[k++] = i;
        if (i != N / i) ds[k++] = N / i;
        for (int j = 0; j < k; j++) {
            if (ds[j] <= 2) continue;
            if (cnt == cap) {
                cap *= 2;
                vals = (uint64_t *)realloc(vals, cap * sizeof(uint64_t));
            }
            vals[cnt++] = A324537_v(ds[j]);
        }
    }
    seq_t result = 1;
    if (cnt > 0) {
        qsort(vals, cnt, sizeof(uint64_t), A324537_cmp);
        size_t i = 0;
        while (i < cnt) {
            size_t j = i;
            while (j < cnt && vals[j] == vals[i]) j++;
            uint64_t e = j - i;
            if (e > 1) {
                seq_t p = (seq_t)A324537_nth(vals[i]);
                for (uint64_t t = 1; t < e; t++) result *= p;
            }
            i = j;
        }
    }
    free(vals);
    return result;
}
