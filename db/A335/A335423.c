/*
 * A335423  a(n) = A005940(1+A248663(n)).
 * https://oeis.org/A335423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A005940(1+A248663(n)): if the odd-exponent prime factors
 *            of n are prime(b_0+1) < prime(b_1+1) < ..., then a(n) =
 *            Product_j prime(1 + b_j - j)
 * Range:     u8 n <= 256, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 106 terms through n = 9932 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

static uint64_t *A335423_primes = NULL;
static size_t    A335423_pcnt   = 0;
static uint64_t  A335423_cap    = 0;

/* make sure all primes <= limit are cached */
static void A335423_ensure(uint64_t limit)
{
    if (limit <= A335423_cap && A335423_primes != NULL) return;
    uint64_t newcap = limit < 1024 ? 1024 : limit;
    if (newcap < A335423_cap * 2) newcap = A335423_cap * 2;
    unsigned char *comp = (unsigned char *)calloc((size_t)newcap + 1, 1);
    size_t cnt = 0;
    for (uint64_t i = 2; i <= newcap; ++i) {
        if (!comp[i]) {
            ++cnt;
            for (uint64_t j = i * i; j <= newcap; j += i) comp[j] = 1;
        }
    }
    uint64_t *pl = (uint64_t *)malloc((cnt ? cnt : 1) * sizeof(uint64_t));
    size_t k = 0;
    for (uint64_t i = 2; i <= newcap; ++i)
        if (!comp[i]) pl[k++] = i;
    free(comp);
    free(A335423_primes);
    A335423_primes = pl;
    A335423_pcnt = cnt;
    A335423_cap = newcap;
}

/* zero-based index of prime p in the cache */
static uint64_t A335423_index(uint64_t p)
{
    size_t lo = 0, hi = A335423_pcnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (A335423_primes[mid] < p) lo = mid + 1;
        else hi = mid;
    }
    return (uint64_t)lo;
}

seq_t a(idx_t n)
{
    seq_t result = 1;
    if (n <= 1) return result;

    uint64_t m = (uint64_t)n;
    A335423_ensure(m);

    uint64_t bits[64];
    size_t nb = 0;
    for (size_t i = 0; i < A335423_pcnt && m > 1; ++i) {
        uint64_t p = A335423_primes[i];
        if (p * p > m) break;
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; ++e; }
            if (e & 1) bits[nb++] = (uint64_t)i;
        }
    }
    if (m > 1) bits[nb++] = A335423_index(m);
    /* bits already ascending since primes were scanned in order */

    for (size_t j = 0; j < nb; ++j) {
        uint64_t idx = bits[j] - (uint64_t)j; /* zero-based prime index */
        result *= (seq_t)A335423_primes[idx];
    }
    return result;
}
