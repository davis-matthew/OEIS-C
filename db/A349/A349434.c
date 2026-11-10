/*
 * A349434  Dirichlet convolution of A129283 (n + its arithmetic derivative)
 *          with A349337 (Dirichlet inverse of A230593).
 * https://oeis.org/A349434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n} A129283(n/d) * A349337(d), where A129283(m) = m
 *            + m' and A349337 is the Dirichlet inverse of A230593
 * Range:     i8 n <= 511, i16 n <= 20000, i32 n <= 20000, i64 n <= 20000,
 *            i128 n <= 20000, big any n
 * Validated: standard, pass, 116 terms through n = 8478 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* A230593(m) = m + sum over distinct primes p|m of m/p */
static seq_t A230593_val(uint64_t m)
{
    seq_t s = (seq_t)m;
    uint64_t t = m;
    for (uint64_t p = 2; p <= t / p; p += (p == 2 ? 1 : 2)) {
        if (t % p == 0) {
            while (t % p == 0) t /= p;
            s += (seq_t)(m / p);
        }
    }
    if (t > 1) s += (seq_t)(m / t);
    return s;
}

/* A129283(m) = m + arithmetic derivative of m */
static seq_t A129283_val(uint64_t m)
{
    seq_t s = (seq_t)m;
    uint64_t t = m;
    for (uint64_t p = 2; p <= t / p; p += (p == 2 ? 1 : 2)) {
        if (t % p == 0) {
            int e = 0;
            while (t % p == 0) { t /= p; e++; }
            s += (seq_t)e * (seq_t)(m / p);
        }
    }
    if (t > 1) s += (seq_t)(m / t);
    return s;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    uint64_t N = (uint64_t)n;

    size_t cap = 4;
    for (uint64_t i = 1; i <= N / i; i++) cap += 2;
    uint64_t *d = (uint64_t *)malloc(cap * sizeof(uint64_t));
    if (!d) exit(1);
    size_t k = 0, small = 0;
    for (uint64_t i = 1; i <= N / i; i++) {
        if (N % i == 0) d[k++] = i;
    }
    small = k;
    for (size_t j = small; j-- > 0; ) {
        if (d[j] != N / d[j]) d[k++] = N / d[j];
    }

    seq_t *B = mem_new(seq_t, k);
    seq_t res = 0;
    for (size_t i = 0; i < k; i++) {
        if (i == 0) {
            B[0] = 1;
        } else {
            seq_t s = 0;
            for (size_t j = 0; j < i; j++) {
                if (d[i] % d[j] == 0)
                    s += A230593_val(d[i] / d[j]) * B[j];
            }
            B[i] = -s;
        }
        res += A129283_val(N / d[i]) * B[i];
    }
    mem_free(B);
    free(d);
    return res;
}
