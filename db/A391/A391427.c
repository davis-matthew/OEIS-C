/*
 * A391427  Cubefree exponential abundant numbers: cubefree numbers k for
 *          which A051377(k) > 2*k.
 * https://oeis.org/A391427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th cubefree k with A051377(k) > 2*k, where A051377 is
 *            multiplicative with f(p)=p, f(p^2)=p*(p+1); computed by a
 *            segmented sieve
 * Range:     u8 none, u16 n <= 49, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 55 terms through n = 9451 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define A391427_B 65536u

static uint64_t *a391427_cache = NULL;
static size_t a391427_len = 0, a391427_cap = 0;
static uint64_t a391427_next = 1;
static uint32_t *a391427_pr = NULL;
static size_t a391427_np = 0;

static void a391427_init_primes(void)
{
    if (a391427_pr) return;
    const uint32_t lim = 65536u;
    char *s = (char *)calloc(lim + 1, 1);
    a391427_pr = (uint32_t *)malloc(sizeof(uint32_t) * 7000);
    for (uint32_t i = 2; i <= lim; i++) {
        if (!s[i]) {
            a391427_pr[a391427_np++] = i;
            for (uint64_t j = (uint64_t)i * i; j <= lim; j += i) s[j] = 1;
        }
    }
    free(s);
}

static void a391427_block(void)
{
    static uint64_t rem[A391427_B], f[A391427_B];
    static unsigned char ok[A391427_B];
    uint64_t L = a391427_next, H = L + A391427_B;
    for (uint32_t i = 0; i < A391427_B; i++) {
        rem[i] = L + i; f[i] = 1; ok[i] = 1;
    }
    for (size_t pi = 0; pi < a391427_np; pi++) {
        uint64_t p = a391427_pr[pi];
        if (p * p >= H) break;
        uint64_t start = ((L + p - 1) / p) * p;
        for (uint64_t m = start; m < H; m += p) {
            uint32_t i = (uint32_t)(m - L);
            int e = 0;
            while (rem[i] % p == 0) { rem[i] /= p; e++; }
            if (e >= 3) ok[i] = 0;
            else if (e == 1) f[i] *= p;
            else f[i] *= p * (p + 1);
        }
    }
    for (uint32_t i = 0; i < A391427_B; i++) {
        if (!ok[i]) continue;
        if (rem[i] > 1) f[i] *= rem[i];
        uint64_t k = L + i;
        if (f[i] > 2 * k) {
            if (a391427_len >= a391427_cap) {
                a391427_cap = a391427_cap ? a391427_cap * 2 : 1024;
                a391427_cache = (uint64_t *)realloc(a391427_cache, a391427_cap * sizeof(uint64_t));
            }
            a391427_cache[a391427_len++] = k;
        }
    }
    a391427_next = H;
}

seq_t a(idx_t n)
{
    a391427_init_primes();
    while (a391427_len < (size_t)n)
        a391427_block();
    return (seq_t)a391427_cache[n - 1];
}
