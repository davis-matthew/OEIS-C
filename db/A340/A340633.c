/*
 * A340633  Primes of the form k + A037276(k).
 * https://oeis.org/A340633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   search, not a closed form. The sequence lists, in increasing
 *            order, the distinct primes v = k + A037276(k). Since A037276(k)
 *            >= k, any k with v <= V satisfies k <= V/2. If k > 1 is odd,
 *            every prime factor is odd, so A037276(k) is odd and v is even,
 *            so only k = 1 and even k can give a prime. For a...
 * Range:     u8 n <= 6, u16 n <= 563, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9475 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static unsigned char *A340633_sieve = 0;   /* 1 = prime, indices 0..A340633_V */
static uint64_t A340633_V = 0;
static uint64_t *A340633_vals = 0;
static size_t A340633_cnt = 0;
static size_t A340633_cap = 0;

static void A340633_push(uint64_t v)
{
    if (A340633_cnt == A340633_cap) {
        size_t nc = A340633_cap ? A340633_cap * 2 : 1024;
        A340633_vals = (uint64_t *)realloc(A340633_vals, nc * sizeof(uint64_t));
        A340633_cap = nc;
    }
    A340633_vals[A340633_cnt++] = v;
}

static uint64_t A340633_pow10_of(uint64_t q)
{
    uint64_t d = 10;
    while (q >= d) d *= 10;
    return d;
}

static void A340633_dfs(uint64_t pmin, uint64_t k, uint64_t c)
{
    uint64_t V = A340633_V;
    uint64_t q;
    for (q = pmin; q <= V; ++q) {
        if (!A340633_sieve[q]) continue;
        uint64_t d = A340633_pow10_of(q);
        if (c > V / d) break;
        uint64_t c2 = c * d + q;
        if (c2 > V) break;
        if (k > (V - c2) / q) break;
        uint64_t k2 = k * q;
        uint64_t v = c2 + k2;
        if (v > V) break;
        if (A340633_sieve[v]) A340633_push(v);
        A340633_dfs(q, k2, c2);
    }
}

static int A340633_cmp(const void *x, const void *y)
{
    uint64_t a1 = *(const uint64_t *)x, b1 = *(const uint64_t *)y;
    return (a1 > b1) - (a1 < b1);
}

static void A340633_build(uint64_t V)
{
    uint64_t i, j;
    free(A340633_sieve);
    A340633_sieve = (unsigned char *)malloc((size_t)(V + 1));
    memset(A340633_sieve, 1, (size_t)(V + 1));
    A340633_sieve[0] = 0;
    if (V >= 1) A340633_sieve[1] = 0;
    for (i = 2; i * i <= V; ++i)
        if (A340633_sieve[i])
            for (j = i * i; j <= V; j += i) A340633_sieve[j] = 0;
    A340633_V = V;
    A340633_cnt = 0;
    if (V >= 2) A340633_push(2);              /* k = 1: 1 + 1 = 2 */
    /* first prime factor must be 2: k = 2 gives c = 2 */
    if (V >= 4) {
        /* k = 2 itself: v = 4, not prime; extend with primes >= 2 */
        A340633_dfs(2, 2, 2);
    }
    if (A340633_cnt > 1)
        qsort(A340633_vals, A340633_cnt, sizeof(uint64_t), A340633_cmp);
    /* dedupe */
    if (A340633_cnt > 1) {
        size_t w = 1, r;
        for (r = 1; r < A340633_cnt; ++r)
            if (A340633_vals[r] != A340633_vals[w - 1])
                A340633_vals[w++] = A340633_vals[r];
        A340633_cnt = w;
    }
}

seq_t a(idx_t n)
{
    size_t want = (size_t)n;
    if (A340633_V == 0) A340633_build(1024);
    while (A340633_cnt < want) A340633_build(A340633_V * 4);
    return (seq_t)A340633_vals[want - 1];
}
