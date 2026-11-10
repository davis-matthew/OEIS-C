/*
 * A378750  Dirichlet inverse of A377984, where A377984(n) = 2*sigma(n) -
 *          A003961(n).
 * https://oeis.org/A378750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1; for n > 1, a(n) = -Sum_{d|n, d<n} A377984(n/d) * a(d),
 *            where A377984(n) = 2*sigma(n) - A003961(n)
 * Range:     i8 n <= 80, i16 n <= 2047, i32 n <= 20000, i64 n <= 20000,
 *            i128 n <= 20000, big any n
 * Validated: standard, pass, 90 terms through n = 8869 (2026-11-10)
 * Helpers:   mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/next_prime.h"
#include "../utils/mem.h"

static seq_t A377984_a(uint64_t n) {
    seq_t sg = 1, a3 = 1;
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            seq_t ppow = 1, sum = 1, np_pow = 1;
            seq_t np = (seq_t)next_prime(p);
            while (m % p == 0) {
                m /= p;
                ppow *= (seq_t)p;
                sum += ppow;
                np_pow *= np;
            }
            sg *= sum;
            a3 *= np_pow;
        }
    }
    if (m > 1) {
        sg *= (seq_t)(m + 1);
        a3 *= (seq_t)next_prime(m);
    }
    return sg * 2 - a3;
}

seq_t a(idx_t n) {
    if (n <= 1) return 1;
    size_t cap = 16, cnt = 0;
    uint64_t *dv = (uint64_t *)malloc(cap * sizeof(uint64_t));
    for (uint64_t i = 1; i * i <= (uint64_t)n; i++) {
        if ((uint64_t)n % i == 0) {
            if (cnt + 2 > cap) {
                cap *= 2;
                dv = (uint64_t *)realloc(dv, cap * sizeof(uint64_t));
            }
            dv[cnt++] = i;
            if (i != (uint64_t)n / i) dv[cnt++] = (uint64_t)n / i;
        }
    }
    /* insertion sort ascending */
    for (size_t i = 1; i < cnt; i++) {
        uint64_t v = dv[i];
        size_t j = i;
        while (j > 0 && dv[j - 1] > v) { dv[j] = dv[j - 1]; j--; }
        dv[j] = v;
    }
    seq_t *fa = mem_new(seq_t, cnt);
    seq_t *A = mem_new(seq_t, cnt);
    for (size_t i = 0; i < cnt; i++) fa[i] = A377984_a(dv[i]);
    for (size_t j = 0; j < cnt; j++) {
        if (j == 0) { A[0] = 1; continue; }
        seq_t s = 0;
        for (size_t i = 0; i < j; i++) {
            if (dv[j] % dv[i] == 0) {
                uint64_t q = dv[j] / dv[i];
                size_t lo = 0, hi = cnt - 1;
                while (lo < hi) {
                    size_t mid = (lo + hi) / 2;
                    if (dv[mid] < q) lo = mid + 1; else hi = mid;
                }
                s += fa[lo] * A[i];
            }
        }
        A[j] = -s;
    }
    seq_t res = A[cnt - 1];
    mem_free(fa);
    mem_free(A);
    free(dv);
    return res;
}
