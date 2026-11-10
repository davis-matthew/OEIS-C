/*
 * A339309  Numbers k such that A228553(k) is twice a prime.
 * https://oeis.org/A339309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with A228553(k) = 2*prime, where A228553(k) =
 *            Sum_{p<=k, p and 2k-p prime} p*(2k-p); found by scanning k
 *            upward with a sieve (results cached)
 * Range:     u8 n <= 22, u16 n <= 1337, u32 n <= 3016, u64 n <= 3016,
 *            u128 n <= 3016, big any n
 * Validated: standard, pass, 76 terms through n = 2815 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static unsigned char *g_flags = NULL;
static uint32_t *g_primes = NULL;
static size_t g_pcnt = 0;
static uint64_t g_limit = 0;

static uint64_t *g_found = NULL;
static size_t g_fcnt = 0, g_fcap = 0;
static uint64_t g_next = 1;

static void g_resieve(uint64_t limit)
{
    free(g_flags);
    free(g_primes);
    g_flags = (unsigned char *)calloc((size_t)limit + 1, 1);
    g_primes = (uint32_t *)malloc(sizeof(uint32_t) * ((size_t)limit / 2 + 2));
    if (!g_flags || !g_primes) abort();
    for (uint64_t i = 2; i <= limit; ++i) g_flags[i] = 1;
    for (uint64_t p = 2; p * p <= limit; ++p)
        if (g_flags[p])
            for (uint64_t q = p * p; q <= limit; q += p) g_flags[q] = 0;
    g_pcnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (g_flags[i]) g_primes[g_pcnt++] = (uint32_t)i;
    g_limit = limit;
}

static uint64_t twice_mulmod(uint64_t a, uint64_t b, uint64_t m)
{
    return (uint64_t)((unsigned __int128)a * b % m);
}

static uint64_t twice_powmod(uint64_t b, uint64_t e, uint64_t m)
{
    uint64_t r = 1;
    b %= m;
    while (e) {
        if (e & 1) r = twice_mulmod(r, b, m);
        b = twice_mulmod(b, b, m);
        e >>= 1;
    }
    return r;
}

/* SHARED_HELPER: deterministic 64-bit Miller-Rabin primality test */
static bool twice_is_prime64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t bs[12] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    for (int i = 0; i < 12; ++i) {
        if (n % bs[i] == 0) return n == bs[i];
    }
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    for (int i = 0; i < 12; ++i) {
        uint64_t x = twice_powmod(bs[i], d, n);
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        for (int r = 1; r < s; ++r) {
            x = twice_mulmod(x, x, n);
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    while (g_fcnt < (size_t)n) {
        uint64_t m = g_next;
        if (2 * m > g_limit) {
            uint64_t nl = g_limit ? g_limit * 2 : 65536;
            while (nl < 2 * m) nl *= 2;
            g_resieve(nl);
        }
        uint64_t S = 0;
        uint64_t twom = 2 * m;
        for (size_t i = 0; i < g_pcnt; ++i) {
            uint64_t p = g_primes[i];
            if (p > m) break;
            if (g_flags[twom - p]) S += p * (twom - p);
        }
        if (S > 0 && (S & 1) == 0 && twice_is_prime64(S / 2)) {
            if (g_fcnt == g_fcap) {
                g_fcap = g_fcap ? g_fcap * 2 : 1024;
                g_found = (uint64_t *)realloc(g_found, g_fcap * sizeof(uint64_t));
                if (!g_found) abort();
            }
            g_found[g_fcnt++] = m;
        }
        ++g_next;
    }
    return (seq_t)g_found[n - 1];
}
