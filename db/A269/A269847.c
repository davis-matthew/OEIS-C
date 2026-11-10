/*
 * A269847  Permutation of natural numbers: a(1) = 1, for n > 1, if n is an
 *          odd prime, a(n) = A003961(a(A000720(n))), otherwise a(n) =
 *          2*a(n-A000720(n)).
 * https://oeis.org/A269847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1, if n is an odd prime, a(n) =
 *            A003961(a(pi(n))), otherwise a(n) = 2*a(n - pi(n)). Computed by
 *            a sieve and a table
 * Range:     u8 n <= 61, u16 n <= 476, u32 n <= 6701, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 91 terms through n = 9918 (2026-11-07)
 * Helpers:   mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"
#include "../utils/next_prime.h"

static seq_t A003961_a(seq_t v) {
    seq_t result = 1;
    seq_t m = v;
    for (uint64_t p = 2; (seq_t)p * (seq_t)p <= m; p++) {
        if (m % p == 0) {
            seq_t q = (seq_t)next_prime(p);
            while (m % p == 0) {
                m /= p;
                result *= q;
            }
        }
    }
    if (m > 1) {
        result *= (seq_t)next_prime((uint64_t)m);
    }
    return result;
}

seq_t a(idx_t n) {
    size_t N = (size_t)n;
    char *comp = (char *)calloc(N + 2, 1);
    size_t *pi = (size_t *)malloc((N + 2) * sizeof(size_t));
    seq_t *t = mem_new(seq_t, N + 2);
    if (!comp || !pi || !t) {
        free(comp);
        free(pi);
        mem_free(t);
        return 0;
    }
    for (size_t i = 2; i * i <= N; i++) {
        if (!comp[i]) {
            for (size_t j = i * i; j <= N; j += i) comp[j] = 1;
        }
    }
    pi[0] = 0;
    if (N >= 1) pi[1] = 0;
    for (size_t i = 2; i <= N; i++) pi[i] = pi[i - 1] + (comp[i] ? 0 : 1);

    t[1] = 1;
    for (size_t i = 2; i <= N; i++) {
        if (!comp[i] && i > 2) {
            t[i] = A003961_a(t[pi[i]]);
        } else {
            t[i] = (seq_t)2 * t[i - pi[i]];
        }
    }
    seq_t r = t[N];
    free(comp);
    free(pi);
    mem_free(t);
    return r;
}
