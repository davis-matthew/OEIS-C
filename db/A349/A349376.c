/*
 * A349376  Dirichlet convolution of A006368 with the Dirichlet inverse of
 *          A006369, where A006368 is the "amusical permutation", and A006369
 *          is its inverse permutation.
 * https://oeis.org/A349376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n} A006368(d) * b(n/d), where b is the Dirichlet
 *            inverse of A006369: b(1)=1, b(n) = -Sum_{d|n, d>1}
 *            A006369(d)*b(n/d)
 * Range:     i8 n <= 59, i16 n <= 1679, i32 n <= 20000, i64 n <= 20000,
 *            i128 n <= 20000, big any n
 * Validated: standard, pass, 92 terms through n = 7965 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t A006368_a(idx_t n) {
    if ((n & 1) == 0) {
        return ((seq_t)n * 3) / 2;
    } else {
        return ((seq_t)n * 3 + 2) / 4;
    }
}

static seq_t A006369_a(idx_t n) {
    seq_t sn = (seq_t)n;
    idx_t r = n % 3;
    if (r == 0) {
        return sn * 2 / 3;
    } else if (r == 1) {
        return (sn * 4 - 1) / 3;
    } else {
        return (sn * 4 + 1) / 3;
    }
}

static size_t A349376_find(const idx_t *ds, size_t cnt, idx_t v) {
    size_t lo = 0, hi = cnt;
    while (lo + 1 < hi) {
        size_t mid = (lo + hi) / 2;
        if (ds[mid] <= v) lo = mid; else hi = mid;
    }
    return lo;
}

seq_t a(idx_t n) {
    idx_t *ds = (idx_t *)malloc(sizeof(idx_t) * 2 * 4200 + 16);
    size_t cap = 2 * 4200;
    size_t cnt = 0;
    idx_t i;
    size_t lowcnt;
    size_t k, j;
    seq_t *b;
    seq_t res;

    /* collect divisors: small ascending, then large */
    {
        idx_t *tmp = (idx_t *)malloc(sizeof(idx_t) * cap);
        size_t tc = 0;
        for (i = 1; i * i <= n; i++) {
            if (n % i == 0) {
                if (cnt + 1 >= cap) {
                    cap *= 2;
                    ds = (idx_t *)realloc(ds, sizeof(idx_t) * cap);
                    tmp = (idx_t *)realloc(tmp, sizeof(idx_t) * cap);
                }
                ds[cnt++] = i;
                if (i != n / i) tmp[tc++] = n / i;
            }
        }
        lowcnt = cnt;
        while (tc > 0) {
            ds[cnt++] = tmp[--tc];
        }
        free(tmp);
    }
    (void)lowcnt;

    b = mem_new(seq_t, cnt);
    for (k = 0; k < cnt; k++) {
        if (ds[k] == 1) {
            b[k] = 1;
            continue;
        }
        seq_t s = 0;
        for (j = 1; j <= k; j++) {
            if (ds[k] % ds[j] == 0) {
                size_t q = A349376_find(ds, cnt, ds[k] / ds[j]);
                s += A006369_a(ds[j]) * b[q];
            }
        }
        b[k] = -s;
    }

    res = 0;
    for (k = 0; k < cnt; k++) {
        size_t q = A349376_find(ds, cnt, n / ds[k]);
        res += A006368_a(ds[k]) * b[q];
    }
    mem_free(b);
    free(ds);
    return res;
}
