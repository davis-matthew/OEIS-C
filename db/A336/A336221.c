/*
 * A336221  Numbers k such that k, k+1, k+2, k+3 and k+4 are terms of A288041.
 * https://oeis.org/A336221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(n) = n-th k such that S(k),...,S(k+4) are all prime,
 *            where S(j) = prime(j)+prime(j+1)+...+prime(j+4); primes are
 *            streamed with a segmented sieve (bounded memory), candidates
 *            skip-scanned from S(k+4) down
 * Range:     u8 none, u16 n <= 5, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 53 terms through n = 899 (2026-11-08)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

#define A336221_S ((size_t)1 << 18)   /* odd numbers per segment */

static uint64_t *A336221_buf = NULL;   /* window of consecutive primes */
static size_t A336221_len = 0;
static uint64_t A336221_k0 = 1;        /* prime index of buf[0] (1-based) */
static uint64_t A336221_seglo = 0;     /* next segment start (even) */
static unsigned char *A336221_sv = NULL;
static uint32_t *A336221_bp = NULL;    /* odd base primes */
static size_t A336221_bpcnt = 0;
static uint64_t A336221_bplim = 0;

static void A336221_base(uint64_t lim)
{
    unsigned char *c = (unsigned char *)calloc((size_t)lim + 1, 1);
    if (!c) abort();
    size_t cnt = 0;
    for (uint64_t i = 3; i <= lim; i += 2) {
        if (!c[i]) {
            cnt++;
            for (uint64_t j = i * i; j <= lim; j += 2 * i) c[j] = 1;
        }
    }
    uint32_t *p = (uint32_t *)realloc(A336221_bp, (cnt ? cnt : 1) * sizeof(uint32_t));
    if (!p) abort();
    size_t m = 0;
    for (uint64_t i = 3; i <= lim; i += 2)
        if (!c[i]) p[m++] = (uint32_t)i;
    free(c);
    A336221_bp = p;
    A336221_bpcnt = cnt;
    A336221_bplim = lim;
}

static void A336221_init(void)
{
    if (A336221_buf) return;
    A336221_buf = (uint64_t *)malloc((A336221_S + 64) * sizeof(uint64_t));
    A336221_sv = (unsigned char *)malloc(A336221_S);
    if (!A336221_buf || !A336221_sv) abort();
    A336221_buf[0] = 2;
    A336221_len = 1;
    A336221_k0 = 1;
    A336221_seglo = 0;
    A336221_base(1u << 16);
}

/* drop primes with index < keep_k, then append one sieved segment */
static void A336221_refill(uint64_t keep_k)
{
    if (keep_k > A336221_k0) {
        size_t off = (size_t)(keep_k - A336221_k0);
        if (off > A336221_len) off = A336221_len;
        memmove(A336221_buf, A336221_buf + off, (A336221_len - off) * sizeof(uint64_t));
        A336221_len -= off;
        A336221_k0 += off;
    }
    uint64_t lo = A336221_seglo;
    uint64_t hi = lo + 2 * (uint64_t)A336221_S;   /* odd n in [lo, hi) */
    while (A336221_bplim * A336221_bplim < hi)
        A336221_base(A336221_bplim * 2);
    unsigned char *sv = A336221_sv;
    memset(sv, 0, A336221_S);
    if (lo == 0) sv[0] = 1; /* 1 is not prime */
    for (size_t i = 0; i < A336221_bpcnt; i++) {
        uint64_t p = A336221_bp[i];
        if (p * p >= hi) break;
        uint64_t m = ((lo + p - 1) / p) * p;
        if (m < p * p) m = p * p;
        if ((m & 1) == 0) m += p;
        for (uint64_t j = (m - lo - 1) / 2; j < A336221_S; j += p) sv[j] = 1;
    }
    for (size_t i = 0; i < A336221_S; i++)
        if (!sv[i]) A336221_buf[A336221_len++] = lo + 2 * i + 1;
    A336221_seglo = hi;
}

static bool A336221_sp(uint64_t s)
{
    if (s < 2) return false;
    if (s % 2 == 0) return s == 2;
    if (s % 3 == 0) return s == 3;
    if (s % 5 == 0) return s == 5;
    if (s % 7 == 0) return s == 7;
    if (s % 11 == 0) return s == 11;
    if (s % 13 == 0) return s == 13;
    return is_prime_u64(s);
}

seq_t a(idx_t n)
{
    static seq_t *vals = NULL;
    static size_t len = 0, cap = 0;
    static uint64_t k = 1;

    A336221_init();
    while (len < (size_t)n) {
        while (A336221_k0 + A336221_len <= k + 8)
            A336221_refill(k);
        const uint64_t *p = A336221_buf + (size_t)(k - A336221_k0);
        int j;
        for (j = 4; j >= 0; j--) {
            uint64_t s = p[j] + p[j + 1] + p[j + 2] + p[j + 3] + p[j + 4];
            if (!A336221_sp(s)) break;
        }
        if (j < 0) {
            if (len == cap) {
                size_t nc = cap ? cap * 2 : 64;
                seq_t *t = mem_resize(seq_t, vals, nc);
                if (!t) return 0;
                vals = t;
                cap = nc;
            }
            vals[len++] = (seq_t)k;
            k++;
        } else {
            k += (uint64_t)j + 1;
        }
    }
    return vals[(size_t)n - 1];
}
