/*
 * A285319  Squarefree numbers n for which A019565(n) < n and A048675(n) is
 *          also squarefree.
 * https://oeis.org/A285319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   search. a(n) is the n-th m >= 2 such that m is squarefree,
 *            A019565(m) < m, and A048675(m) = Sum_{p|m} 2^(PrimePi(p)-1) is
 *            squarefree. The squarefree test on A048675(m) is exact when the
 *            value fits in 64 bits. When it is larger, it only checks for
 *            square factors q^2 with prime q < 10^5
 * Range:     u8 n <= 3, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Validated: partial, pass, 38 terms through n = 38 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static const uint32_t A285319_small_primes[64] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53,
    59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131,
    137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223,
    227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311
};

static uint32_t *A285319_pl = NULL;
static size_t A285319_np = 0;
static uint64_t A285319_L = 0;

static void A285319_sieve(uint64_t lim)
{
    unsigned char *comp;
    uint64_t i, j;
    if (lim <= A285319_L) return;
    if (A285319_pl) free(A285319_pl);
    comp = (unsigned char *)calloc((size_t)lim + 1, 1);
    A285319_np = 0;
    for (i = 2; i <= lim; i++) if (!comp[i]) {
        A285319_np++;
        for (j = i * i; j <= lim; j += i) comp[j] = 1;
    }
    A285319_pl = (uint32_t *)malloc(A285319_np * sizeof(uint32_t));
    A285319_np = 0;
    for (i = 2; i <= lim; i++) if (!comp[i]) A285319_pl[A285319_np++] = (uint32_t)i;
    free(comp);
    A285319_L = lim;
}

static uint64_t A285319_pi(uint64_t p)
{
    size_t lo = 0, hi = A285319_np;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (A285319_pl[mid] < p) lo = mid + 1; else hi = mid;
    }
    return (uint64_t)lo + 1;
}

static uint64_t A285319_isqrt(uint64_t x)
{
    uint64_t r = 0, b = (uint64_t)1 << 62;
    while (b > x) b >>= 2;
    while (b) {
        if (x >= r + b) { x -= r + b; r = (r >> 1) + b; } else r >>= 1;
        b >>= 2;
    }
    return r;
}

static bool A285319_sqfree64(uint64_t k)
{
    size_t i;
    if (k == 0) return false;
    for (i = 0; i < A285319_np; i++) {
        uint64_t p = A285319_pl[i];
        if (p * p * p > k) break;
        if (k % p == 0) {
            k /= p;
            if (k % p == 0) return false;
        }
    }
    if (k > 1) {
        uint64_t s = A285319_isqrt(k);
        if (s * s == k) return false;
    }
    return true;
}

static uint64_t A285319_pow2mod(uint64_t e, uint64_t m)
{
    uint64_t r = 1 % m, b = 2 % m;
    while (e) {
        if (e & 1) r = (uint64_t)(((unsigned __int128)r * b) % m);
        b = (uint64_t)(((unsigned __int128)b * b) % m);
        e >>= 1;
    }
    return r;
}

static bool A285319_ok(uint64_t m)
{
    unsigned __int128 prod = 1;
    uint64_t t = m, es[64], maxe = 0;
    int bit = 0;
    size_t cnt = 0, i;
    if (m < 2) return false;
    while (t) {
        if (t & 1) {
            prod *= A285319_small_primes[bit];
            if (prod >= m) return false;
        }
        t >>= 1;
        bit++;
    }
    if (m > A285319_L) A285319_sieve(2 * m);
    t = m;
    for (i = 0; i < A285319_np; i++) {
        uint64_t p = A285319_pl[i];
        if (p * p > t) break;
        if (t % p == 0) {
            t /= p;
            if (t % p == 0) return false;
            es[cnt++] = (uint64_t)i;
        }
    }
    if (t > 1) es[cnt++] = A285319_pi(t) - 1;
    for (i = 0; i < cnt; i++) if (es[i] > maxe) maxe = es[i];
    if (maxe < 63) {
        uint64_t k = 0;
        for (i = 0; i < cnt; i++) k += (uint64_t)1 << es[i];
        return A285319_sqfree64(k);
    }
    for (i = 0; i < A285319_np; i++) {
        uint64_t q = A285319_pl[i], q2, s = 0;
        size_t j;
        if (q > 100000) break;
        q2 = q * q;
        for (j = 0; j < cnt; j++) s = (s + A285319_pow2mod(es[j], q2)) % q2;
        if (s == 0) return false;
    }
    return true;
}

static uint64_t *A285319_terms = NULL;
static size_t A285319_count = 0, A285319_cap = 0;
static uint64_t A285319_next = 1;

seq_t a(idx_t n)
{
    A285319_sieve(3000000);
    while (A285319_count < n) {
        if (A285319_ok(A285319_next)) {
            if (A285319_count == A285319_cap) {
                A285319_cap = A285319_cap ? 2 * A285319_cap : 1024;
                A285319_terms = (uint64_t *)realloc(A285319_terms, A285319_cap * sizeof(uint64_t));
            }
            A285319_terms[A285319_count++] = A285319_next;
        }
        A285319_next++;
    }
    return (seq_t)A285319_terms[n - 1];
}
