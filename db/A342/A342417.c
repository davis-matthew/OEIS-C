/*
 * A342417  Dirichlet inverse of Čiurlionis sequence, A342002.
 * https://oeis.org/A342417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   Dirichlet inverse of b(n)=A342002(n): a(1)=1/b(1), a(n) =
 *            -(1/b(1)) * Sum_{d|n, d<n} a(d)*b(n/d), with b(1)=1
 * Range:     i8 n <= 26, i16 n <= 593, i32 n <= 11550, i64 n <= 11550,
 *            i128 n <= 11550, big any n
 * Validated: standard, pass, 86 terms through n = 9949 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/mem.h"

static const unsigned A342417_primes[] = {
    2u, 3u, 5u, 7u, 11u, 13u, 17u, 19u, 23u, 29u,
    31u, 37u, 41u, 43u, 47u, 53u, 59u, 61u, 67u, 71u,
    73u, 79u, 83u, 89u, 97u, 101u, 103u, 107u, 109u, 113u
};
#define A342417_PC (sizeof(A342417_primes)/sizeof(A342417_primes[0]))

static seq_t A342002_a(idx_t n)
{
    if (n == 0) return 0;
    idx_t tmp = n;
    seq_t rad = 1;
    unsigned digits[64] = {0};
    size_t idx = 0;
    while (tmp > 0 || idx == 0) {
        if (idx >= A342417_PC) break;
        unsigned p = A342417_primes[idx];
        unsigned d = (unsigned)(tmp % p);
        digits[idx] = d;
        tmp = tmp / p;
        if (d > 0) rad *= (seq_t)p;
        ++idx;
    }
    seq_t result = 0;
    for (size_t j = 0; j < idx; ++j) {
        unsigned d = digits[j];
        if (d == 0) continue;
        seq_t term = (rad / (seq_t)A342417_primes[j]) * (seq_t)d;
        result += term;
    }
    return result;
}

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    seq_t *b = mem_new(seq_t, N + 1);
    seq_t *acc = mem_new(seq_t, N + 1);
    for (size_t k = 1; k <= N; ++k) b[k] = A342002_a((idx_t)k);
    seq_t result = 0;
    for (size_t d = 1; d <= N; ++d) {
        seq_t ad = (d == 1) ? (seq_t)1 : (seq_t)(-acc[d]);   /* b(1) = 1 */
        if (d == N) { result = ad; break; }
        for (size_t m = 2 * d, q = 2; m <= N; m += d, ++q)
            acc[m] += ad * b[q];
    }
    mem_free(b);
    mem_free(acc);
    return result;
}
