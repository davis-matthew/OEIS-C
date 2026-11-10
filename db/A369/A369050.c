/*
 * A369050  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => f(i) = f(j), where f(1) = 1, and for n>1, f(n) = [A003415(n),
 *          A369049(n)], for all i, j >= 1.
 * https://oeis.org/A369050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence transform of f(1)=1,
 *            f(n)=[A003415(n), n mod A003415(n)] (A369049(n)=n mod
 *            A003415(n)); computed by scanning m=1..n with a hash table of
 *            seen pairs
 * Range:     u8 n <= 91, u16 n <= 9450, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 107 terms through n = 9450 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t A003415_der(seq_t n) {
    if (n < 2) return 0;
    seq_t result = 0;
    seq_t rem = n;
    for (seq_t p = 2; p * p <= rem; p++) {
        int e = 0;
        while (rem % p == 0) { e++; rem /= p; }
        if (e > 0) result += (n / p) * (seq_t)e;
    }
    if (rem > 1) result += n / rem;
    return result;
}

seq_t a(idx_t n) {
    if (n <= 1) return 1;
    size_t N = (size_t)n;
    seq_t *D = mem_new(seq_t, N + 1);
    seq_t *R = mem_new(seq_t, N + 1);
    size_t cap = 1;
    while (cap < 2 * N + 2) cap <<= 1;
    size_t *tab = (size_t *)malloc(cap * sizeof(size_t)); /* 0 = empty, else index of first occurrence */
    size_t *ids = (size_t *)malloc((N + 1) * sizeof(size_t));
    for (size_t i = 0; i < cap; i++) tab[i] = 0;
    size_t cnt = 1; /* id 1 for f(1) */
    ids[1] = 1;
    size_t result = 1;
    for (size_t m = 2; m <= N; m++) {
        seq_t sm = (seq_t)m;
        seq_t d = A003415_der(sm);
        seq_t r = sm % d;
        D[m] = d;
        R[m] = r;
        unsigned long long h1 = (unsigned long long)(d % (seq_t)1000003);
        unsigned long long h2 = (unsigned long long)(r % (seq_t)999983);
        size_t pos = (size_t)((h1 * 1000003ULL + h2 * 7919ULL + h1 * h2) & (cap - 1));
        size_t id = 0;
        while (true) {
            size_t t = tab[pos];
            if (t == 0) {
                tab[pos] = m;
                id = ++cnt;
                break;
            }
            if (D[t] == d && R[t] == r) {
                id = ids[t];
                break;
            }
            pos = (pos + 1) & (cap - 1);
        }
        ids[m] = id;
        if (m == N) result = id;
    }
    free(tab);
    free(ids);
    mem_free(D);
    mem_free(R);
    return (seq_t)result;
}
