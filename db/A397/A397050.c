/*
 * A397050  Prime numbers in A397046.
 * https://oeis.org/A397050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th prime among the terms of A397046; found by a direct
 *            search over k, accepting k when digsum(k) is not divisible by
 *            any smaller accepted term, then testing primality
 * Range:     u8 n <= 20, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/mem.h"

static uint64_t p397050_mulmod(uint64_t a, uint64_t b, uint64_t m)
{
    return (uint64_t)((unsigned __int128)a * b % m);
}

static uint64_t p397050_powmod(uint64_t b, uint64_t e, uint64_t m)
{
    uint64_t r = 1;
    b %= m;
    while (e) {
        if (e & 1) r = p397050_mulmod(r, b, m);
        b = p397050_mulmod(b, b, m);
        e >>= 1;
    }
    return r;
}

static bool p397050_is_prime(uint64_t n)
{
    static const uint64_t sp[12] = {2,3,5,7,11,13,17,19,23,29,31,37};
    if (n < 2) return false;
    for (int i = 0; i < 12; i++) {
        if (n == sp[i]) return true;
        if (n % sp[i] == 0) return false;
    }
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; s++; }
    for (int i = 0; i < 12; i++) {
        uint64_t x = p397050_powmod(sp[i], d, n);
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        for (int r = 1; r < s; r++) {
            x = p397050_mulmod(x, x, n);
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

static unsigned p397050_dsum(uint64_t v)
{
    unsigned s = 0;
    while (v) { s += (unsigned)(v % 10); v /= 10; }
    return s;
}

#define P397050_SMALL_MAX 400

static unsigned p397050_small[P397050_SMALL_MAX + 1]; /* A397046 terms <= 400 */
static size_t p397050_nsmall = 0;
static seq_t *p397050_primes = NULL;
static size_t p397050_count = 0;
static size_t p397050_cap = 0;
static uint64_t p397050_next = 2;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    while (p397050_count < (size_t)n) {
        uint64_t k = p397050_next++;
        unsigned ds = p397050_dsum(k);
        bool bad = false;
        for (size_t i = 0; i < p397050_nsmall && p397050_small[i] <= ds; i++) {
            if (ds % p397050_small[i] == 0) { bad = true; break; }
        }
        if (bad) continue;
        if (k <= P397050_SMALL_MAX) p397050_small[p397050_nsmall++] = (unsigned)k;
        if (p397050_is_prime(k)) {
            if (p397050_count + 1 > p397050_cap) {
                size_t nc = p397050_cap ? p397050_cap * 2 : 64;
                p397050_primes = mem_resize(seq_t, p397050_primes, nc);
                p397050_cap = nc;
            }
            p397050_primes[p397050_count++] = (seq_t)k;
        }
    }
    return p397050_primes[(size_t)(n - 1)];
}
