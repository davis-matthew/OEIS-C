/*
 * A342069  a(n) is the smallest k such that A342068(k) = n.
 * https://oeis.org/A342069
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that A342068(k) = n; computed by search
 *            over k, with A342068(k) found via an odd-only prime bitset sieve
 *            and prefix counts
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 42 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *A342069_bits = NULL;
static uint32_t *A342069_cum = NULL;
static uint64_t A342069_lim = 0;

static void A342069_ensure(uint64_t need)
{
    if (need <= A342069_lim) return;
    uint64_t L = A342069_lim ? A342069_lim * 2 : ((uint64_t)1 << 22);
    if (L < need) L = need;
    uint64_t nbits = L / 2 + 2;
    uint64_t nw = (nbits + 63) / 64 + 1;
    free(A342069_bits);
    free(A342069_cum);
    A342069_bits = (uint64_t *)malloc(nw * sizeof(uint64_t));
    A342069_cum = (uint32_t *)malloc(nw * sizeof(uint32_t));
    if (!A342069_bits || !A342069_cum) abort();
    memset(A342069_bits, 0xFF, nw * sizeof(uint64_t));
    A342069_bits[0] &= ~(uint64_t)1; /* number 1 is not prime */
    uint64_t maxidx = (L - 1) / 2;
    for (uint64_t i = 1; ; i++) {
        uint64_t p = 2 * i + 1;
        if (p * p > L) break;
        if (A342069_bits[i >> 6] >> (i & 63) & 1) {
            for (uint64_t j = (p * p) >> 1; j <= maxidx; j += p)
                A342069_bits[j >> 6] &= ~((uint64_t)1 << (j & 63));
        }
    }
    uint32_t c = 0;
    for (uint64_t w = 0; w < nw; w++) {
        A342069_cum[w] = c;
        c += (uint32_t)__builtin_popcountll(A342069_bits[w]);
    }
    A342069_lim = L;
}

static uint64_t A342069_pi(uint64_t x)
{
    if (x < 2) return 0;
    uint64_t idx = (x - 1) / 2;
    uint64_t w = idx >> 6;
    uint64_t mask = ~(uint64_t)0 >> (63 - (idx & 63));
    return 1 + A342069_cum[w] + (uint64_t)__builtin_popcountll(A342069_bits[w] & mask);
}

static uint64_t A342069_f(uint64_t k)
{
    A342069_ensure(2 * k + 2);
    uint64_t c1 = A342069_pi(k);
    for (uint64_t m = 2; ; m++) {
        A342069_ensure(m * k + 2);
        uint64_t c2 = A342069_pi(m * k) - A342069_pi((m - 1) * k);
        if (c2 > c1) return m;
        c1 = c2;
    }
}

#define A342069_CAP 1024
static uint64_t A342069_first[A342069_CAP];
static uint64_t A342069_next = 1;

seq_t a(idx_t n)
{
    if (n >= A342069_CAP) {
        for (uint64_t k = 1; ; k++)
            if (A342069_f(k) == (uint64_t)n) return (seq_t)k;
    }
    while (A342069_first[n] == 0) {
        uint64_t k = A342069_next++;
        uint64_t v = A342069_f(k);
        if (v < A342069_CAP && A342069_first[v] == 0) A342069_first[v] = k;
    }
    return (seq_t)A342069_first[n];
}
