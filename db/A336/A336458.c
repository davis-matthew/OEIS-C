/*
 * A336458  Numbers k for which A065330(k) = A065330(sigma(k)).
 * https://oeis.org/A336458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: k is a term iff A065330(k) = A065330(sigma(k)), i.e. k
 *            and sigma(k) are equal once all factors 2 and 3 are removed;
 *            sigma is computed with a segmented sieve over k = 1, 2, 3,
 * Range:     u8 n <= 10, u16 n <= 23, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

#define A336458_SEG 65536

static uint64_t *A336458_found = NULL;
static size_t A336458_cnt = 0, A336458_cap = 0;
static uint64_t A336458_lo = 1;           /* next segment start */

static uint64_t *A336458_primes = NULL;
static size_t A336458_pcnt = 0;
static uint64_t A336458_plim = 0;         /* primes known up to this bound */

static uint64_t A336458_rem[A336458_SEG];
static uint64_t A336458_sig[A336458_SEG];

static uint64_t A336458_strip(uint64_t x)
{
    while (x % 2 == 0) x /= 2;
    while (x % 3 == 0) x /= 3;
    return x;
}

static void A336458_ensure_primes(uint64_t need)
{
    if (A336458_plim >= need) return;
    uint64_t lim = need * 2 + 1000;
    unsigned char *comp = (unsigned char *)calloc((size_t)lim + 1, 1);
    if (!comp) exit(EXIT_FAILURE);
    size_t cnt = 0;
    for (uint64_t i = 2; i <= lim; ++i) {
        if (!comp[i]) {
            ++cnt;
            for (uint64_t j = i * i; j <= lim; j += i) comp[j] = 1;
        }
    }
    uint64_t *pr = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!pr) exit(EXIT_FAILURE);
    size_t c = 0;
    for (uint64_t i = 2; i <= lim; ++i) if (!comp[i]) pr[c++] = i;
    free(comp);
    free(A336458_primes);
    A336458_primes = pr;
    A336458_pcnt = cnt;
    A336458_plim = lim;
}

static void A336458_push(uint64_t k)
{
    if (A336458_cnt == A336458_cap) {
        size_t nc = A336458_cap ? A336458_cap * 2 : 64;
        uint64_t *t = (uint64_t *)realloc(A336458_found, nc * sizeof(uint64_t));
        if (!t) exit(EXIT_FAILURE);
        A336458_found = t;
        A336458_cap = nc;
    }
    A336458_found[A336458_cnt++] = k;
}

static void A336458_segment(void)
{
    uint64_t lo = A336458_lo, hi = lo + A336458_SEG;   /* [lo, hi) */
    uint64_t r = 1;
    while ((r + 1) * (r + 1) < hi) ++r;
    while (r * r >= hi && r > 1) --r;
    A336458_ensure_primes(r + 1);

    for (size_t i = 0; i < A336458_SEG; ++i) {
        A336458_rem[i] = lo + i;
        A336458_sig[i] = 1;
    }
    for (size_t pi = 0; pi < A336458_pcnt; ++pi) {
        uint64_t p = A336458_primes[pi];
        if (p * p >= hi) break;
        uint64_t start = ((lo + p - 1) / p) * p;
        for (uint64_t j = start; j < hi; j += p) {
            size_t i = (size_t)(j - lo);
            uint64_t x = A336458_rem[i], pk = 1, s = 1;
            while (x % p == 0) { x /= p; pk *= p; s += pk; }
            A336458_rem[i] = x;
            A336458_sig[i] *= s;
        }
    }
    for (size_t i = 0; i < A336458_SEG; ++i) {
        uint64_t k = lo + i;
        uint64_t s = A336458_sig[i];
        if (A336458_rem[i] > 1) s *= (A336458_rem[i] + 1);
        if (A336458_strip(k) == A336458_strip(s)) A336458_push(k);
    }
    A336458_lo = hi;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    while (A336458_cnt < (size_t)n) A336458_segment();
    return (seq_t)A336458_found[(size_t)n - 1];
}
