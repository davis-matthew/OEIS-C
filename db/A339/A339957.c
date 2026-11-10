/*
 * A339957  Primes in A240860 (up to sign).
 * https://oeis.org/A339957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th prime among |A240860(m)|, m increasing, where
 *            A240860(m) = Sum_{i=1..m} (-1)^(i+1)*prime(i)^2; computed by an
 *            incremental sieve + primality test
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 655, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 55 terms through n = 8979 (2026-11-10)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"

static uint32_t *g_pr = NULL;
static size_t g_pcnt = 0;
static uint64_t g_limit = 0;

static void sieve_to(uint64_t limit)
{
    unsigned char *c = (unsigned char *)calloc((size_t)limit + 1, 1);
    size_t cnt = 0;
    uint64_t i, j;
    if (!c) abort();
    for (i = 2; i <= limit; i++) {
        if (!c[i]) {
            cnt++;
            if (i * i <= limit)
                for (j = i * i; j <= limit; j += i) c[j] = 1;
        }
    }
    free(g_pr);
    g_pr = (uint32_t *)malloc((cnt + 1) * sizeof(uint32_t));
    if (!g_pr) abort();
    cnt = 0;
    for (i = 2; i <= limit; i++)
        if (!c[i]) g_pr[cnt++] = (uint32_t)i;
    g_pcnt = cnt;
    g_limit = limit;
    free(c);
}

static uint64_t *g_found = NULL;
static size_t g_fcnt = 0, g_fcap = 0;
static size_t g_idx = 0;       /* number of primes consumed */
static int64_t g_sum = 0;

seq_t a(idx_t n)
{
    while (g_fcnt < (size_t)n) {
        if (g_idx >= g_pcnt) {
            sieve_to(g_limit ? g_limit * 2 : 1 << 16);
        }
        uint64_t p = g_pr[g_idx];
        g_idx++;
        if (g_idx & 1) g_sum += (int64_t)(p * p);
        else g_sum -= (int64_t)(p * p);
        int64_t v = g_sum < 0 ? -g_sum : g_sum;
        if (v > 1 && is_prime_u64((uint64_t)v)) {
            if (g_fcnt == g_fcap) {
                g_fcap = g_fcap ? g_fcap * 2 : 1024;
                g_found = (uint64_t *)realloc(g_found, g_fcap * sizeof(uint64_t));
                if (!g_found) abort();
            }
            g_found[g_fcnt++] = (uint64_t)v;
        }
    }
    return (seq_t)g_found[(size_t)n - 1];
}
