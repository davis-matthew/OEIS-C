/*
 * A302201  E.g.f.: exp (e.g.f. for the "cusp form" A002408).
 * https://oeis.org/A302201
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   E.g.f. exp(C(x)) with C(x) = Sum_{k>=1} A002408(k) x^k/k!; a(0)
 *            = 1, a(n) = Sum_{k=1..n} binomial(n-1,k-1) * A002408(k) * a(n-k)
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 11, i64 n <= 18, i128 n <= 32,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 199 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mem.h"

static seq_t A002408_a(idx_t n) {
    if (n == 0) return 0;

    seq_t result = 1;
    idx_t temp = n;

    idx_t e2 = 0;
    while (temp % 2 == 0) {
        e2++;
        temp /= 2;
    }
    if (e2 > 0) {
        seq_t pw = 1;
        for (idx_t k = 0; k < e2; k++) pw = pw * 8;
        result = -pw;
    }

    for (idx_t p = 3; p * p <= temp; p += 2) {
        idx_t e = 0;
        while (temp % p == 0) {
            e++;
            temp /= p;
        }
        if (e > 0) {
            seq_t ps = (seq_t)p;
            seq_t p3 = ps * ps * ps;
            seq_t term = 1;
            seq_t p3k = 1;
            for (idx_t k = 1; k <= e; k++) {
                p3k = p3k * p3;
                term = term + p3k;
            }
            result = result * term;
        }
    }

    if (temp > 1) {
        seq_t ps = (seq_t)temp;
        seq_t term = ps * ps * ps + 1;
        result = result * term;
    }

    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    size_t N = (size_t)n;
    seq_t *c = mem_new(seq_t, N + 2);
    seq_t *f = mem_new(seq_t, N + 2);
    seq_t *row = mem_new(seq_t, N + 2);
    for (size_t k = 1; k <= N; k++) c[k] = A002408_a((idx_t)k);
    f[0] = 1;
    row[0] = 1; /* C(0,0) */
    for (size_t m = 1; m <= N; m++) {
        seq_t s = 0;
        for (size_t k = 1; k <= m; k++) {
            s += row[k - 1] * c[k] * f[m - k];
        }
        f[m] = s;
        row[m] = 0;
        for (size_t j = m; j >= 1; j--) row[j] += row[j - 1];
    }
    seq_t res = f[N];
    mem_free(c);
    mem_free(f);
    mem_free(row);
    return res;
}
