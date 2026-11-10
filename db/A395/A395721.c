/*
 * A395721  Largest k < m^2 such that both rad(k) and rad(k+1) divide m, where
 *          rad = A007947 and m = A005117(n), or -1 if such k does not exist.
 * https://oeis.org/A395721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = largest k < m^2 with rad(k) | m and rad(k+1) | m, where m
 *            = A005117(n); -1 if none. Method: odd m gives -1 (k or k+1 is
 *            even). Otherwise list all numbers <= m^2 built from the primes
 *            of m, sort them, and take the largest consecutive pair
 * Range:     i8 n <= 40, i16 n <= 2447, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 9935 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static int cmp_u64_a395721(const void *x, const void *y) {
    uint64_t a = *(const uint64_t *)x, b = *(const uint64_t *)y;
    return (a > b) - (a < b);
}

static bool sqfree_a395721(uint64_t k) {
    if (k % 4 == 0) return false;
    for (uint64_t i = 3; i * i <= k; i += 2)
        if (k % (i * i) == 0) return false;
    return true;
}

seq_t a(idx_t n) {
    uint64_t m = 0;
    idx_t cnt = 0;
    while (cnt < n) {
        m++;
        if (sqfree_a395721(m)) cnt++;
    }
    if (m % 2 == 1) return (seq_t)(-1);

    uint64_t primes[32];
    int np = 0;
    uint64_t t = m;
    for (uint64_t p = 2; p * p <= t; p++) {
        if (t % p == 0) {
            primes[np++] = p;
            while (t % p == 0) t /= p;
        }
    }
    if (t > 1) primes[np++] = t;

    uint64_t limit = m * m;
    size_t sz = 1, cap = 1024;
    uint64_t *v = (uint64_t *)malloc(cap * sizeof(uint64_t));
    v[0] = 1;
    for (int i = 0; i < np; i++) {
        uint64_t p = primes[i];
        size_t cur = sz;
        for (size_t j = 0; j < cur; j++) {
            uint64_t x = v[j];
            while (x <= limit / p) {
                x *= p;
                if (sz == cap) {
                    cap *= 2;
                    v = (uint64_t *)realloc(v, cap * sizeof(uint64_t));
                }
                v[sz++] = x;
            }
        }
    }
    qsort(v, sz, sizeof(uint64_t), cmp_u64_a395721);
    seq_t res = (seq_t)(-1);
    for (size_t i = sz; i-- > 1;) {
        if (v[i] == v[i - 1] + 1) {
            res = (seq_t)v[i - 1];
            break;
        }
    }
    free(v);
    return res;
}
