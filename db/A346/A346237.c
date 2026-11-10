/*
 * A346237  Dirichlet inverse of A005187.
 * https://oeis.org/A346237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   Dirichlet inverse b of f = A005187: b(1) = 1, b(n) = -Sum_{d|n,
 *            d<n} f(n/d)*b(d), computed over the divisors of n
 * Range:     i8 n <= 29, i16 n <= 1189, i32 n <= 16384, i64 n <= 16384,
 *            i128 n <= 16384, big any n
 * Validated: standard, pass, 85 terms through n = 8997 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t A005187_a(idx_t n) {
    seq_t result = 0;
    seq_t power = (seq_t)n;
    while (power > 0) {
        result += power;
        power >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    size_t cnt = 0, k = 0;
    idx_t i;
    for (i = 1; i <= n / i; i++) {
        if (n % i == 0) cnt += (i == n / i) ? 1 : 2;
    }
    idx_t *div = (idx_t *)malloc(cnt * sizeof(idx_t));
    size_t lo = 0, hi = cnt;
    for (i = 1; i <= n / i; i++) {
        if (n % i == 0) {
            div[lo++] = i;
            if (i != n / i) div[--hi] = n / i;
        }
    }
    k = cnt;
    seq_t *b = mem_new(seq_t, k);
    for (size_t x = 0; x < k; x++) {
        seq_t s = 0;
        if (x == 0) {
            b[0] = 1;
            continue;
        }
        for (size_t y = 0; y < x; y++) {
            if (div[x] % div[y] == 0) {
                s += A005187_a(div[x] / div[y]) * b[y];
            }
        }
        b[x] = -s;
    }
    seq_t res = b[k - 1];
    mem_free(b);
    free(div);
    return res;
}
