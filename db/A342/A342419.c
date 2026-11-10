/*
 * A342419  Sum of A342002 and its Dirichlet inverse.
 * https://oeis.org/A342419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A342002(n) + A342417(n), where A342417 is the Dirichlet
 *            inverse of A342002 (b(1)=1/A342002(1), sum_{d|n}
 *            A342002(d)*b(n/d) = 0 for n>1); computed by a divisor sieve
 * Range:     i8 n <= 26, i16 n <= 582, i32 n <= 11550, i64 n <= 11550,
 *            i128 n <= 11550, big any n
 * Validated: standard, pass, 96 terms through n = 9918 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/mem.h"

static const unsigned A342002_primes[] = {
    2u, 3u, 5u, 7u, 11u, 13u, 17u, 19u, 23u, 29u,
    31u, 37u, 41u, 43u, 47u, 53u, 59u, 61u, 67u, 71u,
    73u, 79u, 83u, 89u, 97u, 101u, 103u, 107u, 109u, 113u
};
#define A342002_PRIME_COUNT (sizeof(A342002_primes)/sizeof(A342002_primes[0]))

static seq_t A342002_a(idx_t n)
{
    if (n == 0) return 0;
    idx_t tmp = n;
    seq_t rad = 1;
    unsigned digits[64] = {0};
    size_t idx = 0;
    while (tmp > 0 || idx == 0) {
        if (idx >= A342002_PRIME_COUNT) break;
        unsigned p = A342002_primes[idx];
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
        seq_t term = (rad / (seq_t)A342002_primes[j]) * (seq_t)d;
        result += term;
    }
    return result;
}

static seq_t *A342419_B = NULL;
static idx_t A342419_cap = 0;

static void A342419_build(idx_t N)
{
    if (A342419_B) mem_free(A342419_B);
    seq_t *f = mem_new(seq_t, (size_t)N + 1);
    seq_t *b = mem_new(seq_t, (size_t)N + 1);
    for (idx_t i = 1; i <= N; i++) f[i] = A342002_a(i);
    /* f(1) = 1 so b(1) = 1; b[m] holds accumulated sum until finalized */
    for (idx_t m = 1; m <= N; m++) {
        if (m == 1) b[1] = 1;
        else b[m] = -b[m];
        if (b[m] == 0) continue;
        for (idx_t k = 2 * m, q = 2; k <= N; k += m, q++) {
            b[k] += b[m] * f[q];
        }
    }
    mem_free(f);
    A342419_B = b;
    A342419_cap = N;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n > A342419_cap) {
        idx_t N = A342419_cap * 2;
        if (N < n) N = n;
        if (N < 1024) N = 1024;
        A342419_build(N);
    }
    return A342002_a(n) + A342419_B[(size_t)n];
}
