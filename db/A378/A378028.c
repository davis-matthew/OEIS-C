/*
 * A378028  Positions of records in A377059.
 * https://oeis.org/A378028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = positions of records in A377059; computed by scanning k
 *            upward with an SPF sieve, getting the multiplicative order from
 *            the factorization of phi(k), and recording each k whose
 *            A377059(k) exceeds all earlier values
 * Range:     u8 n <= 13, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-10)
 * Helpers:   mulmod, powmod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mulmod.h"
#include "../utils/powmod_u64.h"

static uint32_t *a378028_spf = NULL;
static uint64_t a378028_lim = 0;
static uint64_t *a378028_terms = NULL;
static size_t a378028_cnt = 0, a378028_cap = 0;
static uint64_t a378028_next = 2;
static uint64_t a378028_best = 0;

static void a378028_sieve(uint64_t lim)
{
    free(a378028_spf);
    a378028_spf = (uint32_t *)calloc((size_t)lim + 1, sizeof(uint32_t));
    for (uint64_t i = 2; i <= lim; i++) {
        if (a378028_spf[i] == 0) {
            for (uint64_t j = i; j <= lim; j += i)
                if (a378028_spf[j] == 0) a378028_spf[j] = (uint32_t)i;
        }
    }
    a378028_lim = lim;
}

static uint64_t a378028_phi(uint64_t n)
{
    uint64_t r = n, m = n;
    while (m > 1) {
        uint64_t p = a378028_spf[m];
        while (m % p == 0) m /= p;
        r = r / p * (p - 1);
    }
    return r;
}

static uint64_t a378028_gcd(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static uint64_t a378028_order(uint64_t x, uint64_t n, uint64_t phi)
{
    uint64_t ord = phi, m = phi;
    while (m > 1) {
        uint64_t q = a378028_spf[m];
        while (m % q == 0) m /= q;
        while (ord % q == 0 && powmod_u64(x, ord / q, n) == 1) ord /= q;
    }
    return ord;
}

static uint64_t a378028_val(uint64_t n)
{
    if (n < 4) return 0;
    uint64_t phi = a378028_phi(n);
    for (uint64_t x = 2; x < n; x++) {
        if (a378028_gcd(x, n) != 1) continue;
        uint64_t r = a378028_order(x, n, phi);
        if ((r % 2) == 0 && r < n - 1) return r;
    }
    return 0;
}

static void a378028_push(uint64_t v)
{
    if (a378028_cnt == a378028_cap) {
        a378028_cap = a378028_cap ? a378028_cap * 2 : 64;
        a378028_terms = (uint64_t *)realloc(a378028_terms, a378028_cap * sizeof(uint64_t));
    }
    a378028_terms[a378028_cnt++] = v;
}

seq_t a(idx_t n)
{
    if (a378028_cnt == 0) a378028_push(1);
    while (a378028_cnt < (size_t)n) {
        if (a378028_next > a378028_lim)
            a378028_sieve(a378028_lim ? a378028_lim * 2 : 1 << 16);
        uint64_t k = a378028_next++;
        uint64_t v = a378028_val(k);
        if (v > a378028_best) {
            a378028_best = v;
            a378028_push(k);
        }
    }
    return (seq_t)a378028_terms[n - 1];
}
