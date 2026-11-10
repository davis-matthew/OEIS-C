/*
 * A241913  Complement of A241912, natural numbers not fixed by A241916.
 * https://oeis.org/A241913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th number m with A241916(m) != m; m is fixed iff m =
 *            2^v or (m = 2^v2 * ... * p^e with e = v2+1 and exponents of odd
 *            primes below p palindromic). Computed by search with cached
 *            terms
 * Range:     u8 n <= 180, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9304 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *A241913_pr = NULL;
static size_t A241913_pc = 0;
static uint64_t A241913_lim = 0;

static void A241913_sieve(uint64_t limit)
{
    if (limit <= A241913_lim) return;
    uint64_t L = limit * 2 + 100;
    unsigned char *comp = (unsigned char *)calloc((size_t)L + 1, 1);
    if (!comp) abort();
    free(A241913_pr);
    A241913_pr = (uint32_t *)malloc(sizeof(uint32_t) * ((size_t)L / 2 + 10));
    if (!A241913_pr) abort();
    size_t c = 0;
    for (uint64_t i = 2; i <= L; ++i) {
        if (!comp[i]) {
            A241913_pr[c++] = (uint32_t)i;
            for (uint64_t j = i * i; j <= L; j += i) comp[j] = 1;
        }
    }
    free(comp);
    A241913_pc = c;
    A241913_lim = L;
}

static size_t A241913_index(uint32_t p)
{
    size_t lo = 0, hi = A241913_pc;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (A241913_pr[mid] < p) lo = mid + 1; else hi = mid;
    }
    return lo;
}

static bool A241913_fixed(uint64_t m)
{
    if (m == 1) return true;
    A241913_sieve(m);
    int v2 = 0;
    uint64_t r = m;
    while ((r & 1) == 0) { r >>= 1; ++v2; }
    if (r == 1) return true;

    /* factor odd part */
    unsigned char *ex = (unsigned char *)calloc(A241913_pc + 1, 1);
    if (!ex) abort();
    size_t last = 0;
    int ek = 0;
    for (size_t i = 1; i < A241913_pc; ++i) {
        uint64_t p = A241913_pr[i];
        if (p * p > r) break;
        if (r % p == 0) {
            int e = 0;
            while (r % p == 0) { r /= p; ++e; }
            ex[i] = (unsigned char)e;
            last = i;
            ek = e;
        }
    }
    if (r > 1) {
        last = A241913_index((uint32_t)r);
        ex[last] = 1;
        ek = 1;
    }
    bool ok = (ek == v2 + 1);
    if (ok) {
        size_t l = 1, rr = last - 1;
        while (l < rr) {
            if (ex[l] != ex[rr]) { ok = false; break; }
            ++l; --rr;
        }
    }
    free(ex);
    return ok;
}

seq_t a(idx_t n)
{
    static uint64_t *cache = NULL;
    static size_t len = 0, cap = 0;
    static uint64_t next = 1;
    if (cap == 0) {
        cap = 64;
        cache = (uint64_t *)malloc(cap * sizeof(uint64_t));
        if (!cache) abort();
    }
    while (len < (size_t)n) {
        if (!A241913_fixed(next)) {
            if (len >= cap) {
                cap *= 2;
                cache = (uint64_t *)realloc(cache, cap * sizeof(uint64_t));
                if (!cache) abort();
            }
            cache[len++] = next;
        }
        ++next;
    }
    return (seq_t)cache[(size_t)n - 1];
}
