/*
 * A368404  Starts of runs of 4 consecutive integers in A268375.
 * https://oeis.org/A368404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th x such that x, x+1, x+2, x+3 are all in A268375
 *            (exponents of prime factorization have pairwise disjoint binary
 *            representations); found by search over the element 2*s^2 (s odd)
 *            in each run of 4
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 26, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 30 terms through n = 30 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static uint32_t *A368404_pr = NULL;
static size_t A368404_pc = 0;

static void A368404_init(void)
{
    if (A368404_pr) return;
    const size_t L = 1u << 20;
    bool *c = (bool *)malloc(L + 1);
    if (!c) abort();
    for (size_t i = 0; i <= L; i++) c[i] = true;
    c[0] = c[1] = false;
    for (size_t i = 2; i * i <= L; i++)
        if (c[i]) for (size_t j = i * i; j <= L; j += i) c[j] = false;
    size_t cnt = 0;
    for (size_t i = 2; i <= L; i++) if (c[i]) cnt++;
    A368404_pr = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!A368404_pr) abort();
    size_t k = 0;
    for (size_t i = 2; i <= L; i++) if (c[i]) A368404_pr[k++] = (uint32_t)i;
    A368404_pc = cnt;
    free(c);
}

/* membership in A268375 */
static bool A368404_ok(uint64_t t)
{
    uint64_t used = 0;
    unsigned e = 0;
    while ((t & 1) == 0) { t >>= 1; e++; }
    if (e) used |= e;
    for (size_t i = 1; i < A368404_pc; i++) {
        uint64_t p = A368404_pr[i];
        if (p * p > t) break;
        if (t % p == 0) {
            unsigned x = 0;
            while (t % p == 0) { t /= p; x++; }
            if (used & x) return false;
            used |= x;
        }
    }
    if (t > 1 && A368404_pr[A368404_pc - 1] * (uint64_t)A368404_pr[A368404_pc - 1] < t) {
        /* continue with odd numbers beyond the prime table */
        for (uint64_t p = (uint64_t)A368404_pr[A368404_pc - 1] + 2; p * p <= t; p += 2) {
            if (t % p == 0) {
                unsigned x = 0;
                while (t % p == 0) { t /= p; x++; }
                if (used & x) return false;
                used |= x;
            }
        }
    }
    if (t > 1) {
        if (used & 1u) return false;
    }
    return true;
}

/* check that 2*s^2 (s odd) is in A268375 */
static bool A368404_pre(uint64_t s)
{
    uint64_t used = 1, t = s;
    for (uint64_t p = 3; p * p <= t; p += 2) {
        if (t % p == 0) {
            unsigned e = 0;
            while (t % p == 0) { t /= p; e++; }
            uint64_t f = 2ULL * e;
            if (used & f) return false;
            used |= f;
        }
    }
    if (t > 1 && (used & 2ULL)) return false;
    return true;
}

static uint64_t *A368404_cache = NULL;
static size_t A368404_len = 0, A368404_cap = 0;
static uint64_t A368404_s = 1;

seq_t a(idx_t n)
{
    A368404_init();
    while (A368404_len < (size_t)n) {
        uint64_t s = A368404_s;
        A368404_s += 2;
        if (!A368404_pre(s)) continue;
        uint64_t m = 2 * s * s;
        for (int k = 3; k >= 0; k--) {
            if (m < (uint64_t)k + 1) continue;
            uint64_t x = m - (uint64_t)k;
            bool good = true;
            for (int i = 0; i < 4 && good; i++) {
                if (x + (uint64_t)i == m) continue;
                if (!A368404_ok(x + (uint64_t)i)) good = false;
            }
            if (good) {
                if (A368404_len == A368404_cap) {
                    A368404_cap = A368404_cap ? A368404_cap * 2 : 16;
                    uint64_t *tmp = (uint64_t *)realloc(A368404_cache, A368404_cap * sizeof(uint64_t));
                    if (!tmp) abort();
                    A368404_cache = tmp;
                }
                A368404_cache[A368404_len++] = x;
            }
        }
    }
    return (seq_t)A368404_cache[n - 1];
}
