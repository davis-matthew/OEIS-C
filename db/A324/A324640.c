/*
 * A324640  Dirichlet inverse of the Doudna sequence, A005940.
 * https://oeis.org/A324640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1; for n > 1, a(n) = -Sum_{d|n, d<n} a(d) * A005940(n/d),
 *            where A005940(n) = f(n-1, 1, 1), f(0, i, x) = x, f(2m, i, x) =
 *            f(m, i+1, x), f(2m+1, i, x) = f(m, i, x*prime(i)). Computed with
 *            a divisor-sum sieve over all n up to the requested index
 * Range:     i8 n <= 46, i16 n <= 440, i32 n <= 8176, i64 n <= 16384,
 *            i128 n <= 16384, big any n
 * Validated: standard, pass, 96 terms through n = 9823 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/mem.h"

static uint64_t A324640_nth_prime(idx_t i) {
    uint64_t c = 1;
    idx_t cnt = 0;
    while (cnt < i) {
        c++;
        bool ok = true;
        for (uint64_t q = 2; q * q <= c; q++) {
            if (c % q == 0) { ok = false; break; }
        }
        if (ok) cnt++;
    }
    return c;
}

static seq_t A005940_a(idx_t n) {
    idx_t m = n - 1;
    seq_t result = 1;
    idx_t i = 1;
    while (m > 0) {
        if (m % 2 == 1) {
            result *= (seq_t)A324640_nth_prime(i);
            m = (m - 1) / 2;
        } else {
            m /= 2;
            i++;
        }
    }
    return result;
}

static seq_t *A324640_tab = NULL;
static idx_t A324640_cap = 0;

seq_t a(idx_t n) {
    if (n > A324640_cap) {
        idx_t N = n;
        if (N < 2 * A324640_cap) N = 2 * A324640_cap;
        if (N < 64) N = 64;
        if (A324640_tab) mem_free(A324640_tab);
        seq_t *A = mem_new(seq_t, (size_t)N + 1);
        seq_t *D = mem_new(seq_t, (size_t)N + 1);
        for (idx_t k = 1; k <= N; k++) D[k] = A005940_a(k);
        A[1] = 1;
        for (idx_t d = 1; d <= N; d++) {
            if (d > 1) A[d] = -A[d];
            for (idx_t k = 2; d * k <= N; k++) {
                A[d * k] += A[d] * D[k];
            }
        }
        mem_free(D);
        A324640_tab = A;
        A324640_cap = N;
    }
    return A324640_tab[n];
}
