/*
 * A324191  Number of divisors of n minus the number of distinct values that
 *          A297167 obtains over the divisors > 1 of n: a(n) = A000005(n) -
 *          A324190(n).
 * https://oeis.org/A324191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000005(n) - A324190(n), where A324190(n) is the number
 *            of distinct values of A297167(d) over divisors d>1 of n;
 *            A297167(d) = Omega(d) - omega(d) + pi(lpf(d)) - 1. Computed by
 *            enumerating divisors from the factorization
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9660 (2026-11-08)
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/prime_pi.h"

static unsigned long long A324191_pidx[24];
static unsigned A324191_ex[24];
static int A324191_np;
static unsigned long long *A324191_vals;
static size_t A324191_nv;

static void A324191_dfs(int i, unsigned long long sumE, unsigned long long cnt, unsigned long long lastIdx) {
    if (i == A324191_np) {
        if (sumE > 0) {
            A324191_vals[A324191_nv++] = sumE - cnt + lastIdx - 1;
        }
        return;
    }
    A324191_dfs(i + 1, sumE, cnt, lastIdx);
    for (unsigned e = 1; e <= A324191_ex[i]; e++) {
        A324191_dfs(i + 1, sumE + e, cnt + 1, A324191_pidx[i]);
    }
}

static int A324191_cmp(const void *x, const void *y) {
    unsigned long long a = *(const unsigned long long *)x;
    unsigned long long b = *(const unsigned long long *)y;
    return (a > b) - (a < b);
}

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)1;
    idx_t m = n;
    A324191_np = 0;
    size_t d = 1;
    for (idx_t p = 2; p <= m / p; p++) {
        if (m % p == 0) {
            unsigned e = 0;
            while (m % p == 0) { m /= p; e++; }
            A324191_pidx[A324191_np] = (unsigned long long)prime_pi(p);
            A324191_ex[A324191_np] = e;
            A324191_np++;
            d *= (e + 1);
        }
    }
    if (m > 1) {
        A324191_pidx[A324191_np] = (unsigned long long)prime_pi(m);
        A324191_ex[A324191_np] = 1;
        A324191_np++;
        d *= 2;
    }
    A324191_vals = (unsigned long long *)malloc(d * sizeof(unsigned long long));
    A324191_nv = 0;
    A324191_dfs(0, 0, 0, 0);
    qsort(A324191_vals, A324191_nv, sizeof(unsigned long long), A324191_cmp);
    size_t distinct = 0;
    for (size_t i = 0; i < A324191_nv; i++) {
        if (i == 0 || A324191_vals[i] != A324191_vals[i - 1]) distinct++;
    }
    free(A324191_vals);
    return (seq_t)(d - distinct);
}
