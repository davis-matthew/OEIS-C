/*
 * A377556  E.g.f.: exp(Sum_{n>=1} A006519(n) * x^n).
 * https://oeis.org/A377556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   E.g.f. A(x)=exp(B(x)), B(x)=Sum_{k>=1} A006519(k) x^k; a(0)=1,
 *            a(n) = Sum_{k=1..n} binomial(n-1,k-1) * k! * A006519(k) * a(n-k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 21 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t A006519_a(idx_t n) {
    seq_t x = (seq_t)n;
    return x & (-x);
}

seq_t a(idx_t n) {
    size_t N = (size_t)n;
    seq_t *av = mem_new(seq_t, N + 2);
    seq_t *w = mem_new(seq_t, N + 2);
    seq_t *row = mem_new(seq_t, N + 2);
    seq_t fact = 1;
    for (size_t k = 1; k <= N; k++) {
        fact = fact * (seq_t)k;
        w[k] = fact * A006519_a((idx_t)k);
    }
    av[0] = 1;
    row[0] = 1;
    for (size_t m = 1; m <= N; m++) {
        seq_t s = 0;
        for (size_t k = 1; k <= m; k++) {
            s += row[k - 1] * w[k] * av[m - k];
        }
        av[m] = s;
        for (size_t j = m; j >= 1; j--) row[j] += row[j - 1];
    }
    seq_t r = av[N];
    mem_free(av);
    mem_free(w);
    mem_free(row);
    return r;
}
