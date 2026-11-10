/*
 * A347096  a(1) = 1; a(n) = -Sum_{d|n, d < n} A341512(n/d) * a(d), where
 *          A341512(n) = sigma(n)*A003961(n) - n*sigma(A003961(n)).
 * https://oeis.org/A347096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(1) = 1; a(n) = -Sum_{d|n, d<n} A341512(n/d)*a(d), A341512(n) =
 *            sigma(n)*A003961(n) - n*sigma(A003961(n))
 * Range:     i8 n <= 11, i16 n <= 119, i32 n <= 5183, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 8134 (2026-11-10)
 * Helpers:   mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"
#include "../utils/next_prime.h"

static seq_t *A347096_tab = NULL;
static size_t A347096_cap = 0;

static seq_t A347096_b(uint64_t k)
{
    seq_t sig = 1, A = 1, sigA = 1;
    uint64_t m = k;
    uint64_t p;
    for (p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t q = next_prime(p);
            seq_t ps = 1, pp = 1, qs = 1, qp = 1;
            while (m % p == 0) {
                m /= p;
                pp *= (seq_t)p;
                ps += pp;
                qp *= (seq_t)q;
                qs += qp;
                A *= (seq_t)q;
            }
            sig *= ps;
            sigA *= qs;
        }
    }
    if (m > 1) {
        uint64_t q = next_prime(m);
        sig *= ((seq_t)m + 1);
        sigA *= ((seq_t)q + 1);
        A *= (seq_t)q;
    }
    return sig * A - (seq_t)k * sigA;
}

static void A347096_build(size_t N)
{
    size_t d, m, k;
    seq_t *b = mem_new(seq_t, N + 1);
    seq_t *acc = mem_new(seq_t, N + 1);
    seq_t *res = mem_new(seq_t, N + 1);
    for (k = 1; k <= N; k++)
        b[k] = A347096_b((uint64_t)k);
    for (d = 1; d <= N; d++) {
        if (d == 1) res[d] = 1;
        else res[d] = -acc[d];
        for (m = 2 * d, k = 2; m <= N; m += d, k++)
            acc[m] += b[k] * res[d];
    }
    mem_free(b);
    mem_free(acc);
    if (A347096_tab) mem_free(A347096_tab);
    A347096_tab = res;
    A347096_cap = N;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if ((size_t)n > A347096_cap) {
        size_t N = A347096_cap * 2;
        if (N < 1024) N = 1024;
        if (N < (size_t)n) N = (size_t)n;
        A347096_build(N);
    }
    return A347096_tab[(size_t)n];
}
