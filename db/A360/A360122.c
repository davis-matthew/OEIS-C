/*
 * A360122  Parity of A360121, where A360121 is Dirichlet convolution of
 *          A342001 with A342002.
 * https://oeis.org/A360122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A360121(n) mod 2, where A360121(n) = Sum_{d|n}
 *            A342001(d)*A342002(n/d)
 * Range:     u8 n <= 2753, u16 n <= 30030, u32 n <= 30030, u64 n <= 30030,
 *            u128 n <= 30030, big any n
 * Validated: standard, pass, 137 terms through n = 9665 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#define A360122_MAXF 64

static seq_t A342001_a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;
    idx_t primes_[A360122_MAXF];
    idx_t exps_[A360122_MAXF];
    size_t fcnt = 0;
    idx_t t = n;
    for (idx_t p = 2; p <= t / p; ++p) {
        if (t % p == 0) {
            idx_t e = 0;
            while (t % p == 0) { t /= p; ++e; }
            if (fcnt < A360122_MAXF) { primes_[fcnt] = p; exps_[fcnt] = e; ++fcnt; }
        }
    }
    if (t > 1 && fcnt < A360122_MAXF) {
        primes_[fcnt] = t; exps_[fcnt] = (idx_t)1; ++fcnt;
    }
    seq_t rad = (seq_t)1;
    for (size_t i = 0; i < fcnt; ++i)
        rad *= (seq_t)primes_[i];
    seq_t result = (seq_t)0;
    for (size_t i = 0; i < fcnt; ++i) {
        seq_t term = rad / (seq_t)primes_[i];
        result += (seq_t)exps_[i] * term;
    }
    return result;
}

static const unsigned A342002_primes[] = {
    2u, 3u, 5u, 7u, 11u, 13u, 17u, 19u, 23u, 29u,
    31u, 37u, 41u, 43u, 47u, 53u, 59u, 61u, 67u, 71u,
    73u, 79u, 83u, 89u, 97u, 101u, 103u, 107u, 109u, 113u
};
#define A342002_PCOUNT (sizeof(A342002_primes)/sizeof(A342002_primes[0]))

static seq_t A342002_a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    idx_t tmp = n;
    seq_t rad = (seq_t)1;
    unsigned digits[64] = {0};
    size_t idx = 0;
    while (tmp > 0 || idx == 0) {
        if (idx >= A342002_PCOUNT) break;
        unsigned p = A342002_primes[idx];
        unsigned d = (unsigned)(tmp % p);
        digits[idx] = d;
        tmp = tmp / p;
        if (d > 0) rad *= (seq_t)p;
        ++idx;
    }
    seq_t result = (seq_t)0;
    for (size_t j = 0; j < idx; ++j) {
        unsigned d = digits[j];
        if (d == 0) continue;
        seq_t term = (rad / (seq_t)A342002_primes[j]) * (seq_t)d;
        result += term;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t total = (seq_t)0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d != 0) continue;
        idx_t e = n / d;
        total += A342001_a(d) * A342002_a(e);
        if (e != d)
            total += A342001_a(e) * A342002_a(d);
    }
    return (seq_t)(total % (seq_t)2);
}
