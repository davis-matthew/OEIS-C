/*
 * A334107  a(n) = A329697(A122111(n)).
 * https://oeis.org/A334107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A329697(A122111(n)). A122111(n) is built as a
 *            prime-exponent vector (conjugate partition of the prime indices
 *            of n). A329697 of prod p^e is then the sum of e over odd primes
 *            p, each odd prime p also adding the odd prime factors of p-1,
 *            processed from the largest prime down
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9593 (2026-11-08)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/factor_u64.h"

/* index of prime p (p prime): number of primes <= p, via a sieve */
static uint64_t A334107_prime_index(uint64_t p)
{
    if (p < 2) return 0;
    unsigned char *comp = (unsigned char *)calloc((size_t)p + 1, 1);
    if (!comp) abort();
    uint64_t cnt = 0;
    for (uint64_t i = 2; i <= p; ++i) {
        if (comp[i]) continue;
        ++cnt;
        for (uint64_t j = i * i; j <= p; j += i) comp[j] = 1;
    }
    free(comp);
    return cnt;
}

static bool A334107_is_prime_small(uint64_t x)
{
    if (x < 2) return false;
    for (uint64_t d = 2; d * d <= x; ++d)
        if (x % d == 0) return false;
    return true;
}

/* k-th prime, k >= 1 (k is at most 64 here) */
static uint64_t A334107_nth_prime(uint64_t k)
{
    uint64_t c = 0, x = 1;
    while (c < k) {
        ++x;
        if (A334107_is_prime_small(x)) ++c;
    }
    return x;
}

typedef struct { uint64_t idx; uint64_t ex; } A334107_grp;

#define A334107_LIM 400

seq_t a(idx_t n)
{
    uint64_t pr[64], ex[64];
    size_t pc = 0;
    if (n <= 1) return 0;
    factor_u64((uint64_t)n, pr, ex, &pc);

    A334107_grp g[64];
    uint64_t total = 0;
    for (size_t i = 0; i < pc; ++i) {
        g[i].idx = A334107_prime_index(pr[i]);
        g[i].ex = ex[i];
        total += ex[i];
    }
    /* insertion sort by idx */
    for (size_t i = 1; i < pc; ++i) {
        A334107_grp t = g[i];
        size_t j = i;
        while (j > 0 && g[j - 1].idx > t.idx) { g[j] = g[j - 1]; --j; }
        g[j] = t;
    }

    uint64_t e[A334107_LIM];
    for (int i = 0; i < A334107_LIM; ++i) e[i] = 0;

    /* conjugate partition: prime(remaining)^(c_i - c_{i-1}) */
    uint64_t remaining = total, prev = 0;
    for (size_t pos = 0; pos < pc; ++pos) {
        uint64_t cur = g[pos].idx;
        uint64_t length = cur - prev;
        prev = cur;
        uint64_t p = A334107_nth_prime(remaining);
        if (p != 2) e[p] += length;
        remaining -= g[pos].ex;
    }

    /* A329697 on the exponent vector */
    uint64_t steps = 0;
    for (int p = A334107_LIM - 1; p >= 3; --p) {
        if (e[p] == 0) continue;
        uint64_t c = e[p];
        e[p] = 0;
        steps += c;
        int m = p - 1;
        for (int f = 2; f <= m; ++f) {
            while (m % f == 0) {
                m /= f;
                if (f != 2) e[f] += c;
            }
        }
    }
    return (seq_t)steps;
}
