/*
 * A360121  Dirichlet convolution of A342001 with A342002.
 * https://oeis.org/A360121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A342001(n/d) * A342002(d)
 * Range:     u8 n <= 53, u16 n <= 1889, u32 n <= 11550, u64 n <= 11550,
 *            u128 n <= 11550, big any n
 * Validated: standard, pass, 97 terms through n = 9725 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static seq_t A342001_a(idx_t n)
{
    if (n <= 1) return (seq_t)0;
    idx_t pr[64];
    idx_t ex[64];
    size_t f = 0;
    idx_t t = n;
    for (idx_t p = 2; p <= t / p; ++p) {
        if (t % p == 0) {
            idx_t e = 0;
            while (t % p == 0) { t /= p; ++e; }
            if (f < 64) { pr[f] = p; ex[f] = e; ++f; }
        }
    }
    if (t > 1 && f < 64) { pr[f] = t; ex[f] = 1; ++f; }
    seq_t rad = 1;
    for (size_t i = 0; i < f; ++i) rad *= (seq_t)pr[i];
    seq_t res = 0;
    for (size_t i = 0; i < f; ++i)
        res += (seq_t)ex[i] * (rad / (seq_t)pr[i]);
    return res;
}

static const unsigned A342002_primes[] = {
    2u, 3u, 5u, 7u, 11u, 13u, 17u, 19u, 23u, 29u,
    31u, 37u, 41u, 43u, 47u, 53u, 59u, 61u, 67u, 71u,
    73u, 79u, 83u, 89u, 97u, 101u, 103u, 107u, 109u, 113u
};

static seq_t A342002_a(idx_t n)
{
    if (n == 0) return 0;
    idx_t tmp = n;
    seq_t rad = 1;
    unsigned digits[64] = {0};
    size_t idx = 0;
    while (tmp > 0) {
        if (idx >= 30) break;
        unsigned p = A342002_primes[idx];
        unsigned d = (unsigned)(tmp % p);
        digits[idx] = d;
        tmp /= p;
        if (d > 0) rad *= (seq_t)p;
        ++idx;
    }
    seq_t res = 0;
    for (size_t j = 0; j < idx; ++j) {
        if (digits[j] == 0) continue;
        res += (rad / (seq_t)A342002_primes[j]) * (seq_t)digits[j];
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d != 0) continue;
        idx_t e = n / d;
        total += A342001_a(e) * A342002_a(d);
        if (e != d)
            total += A342001_a(d) * A342002_a(e);
    }
    return total;
}
