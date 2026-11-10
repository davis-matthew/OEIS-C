/*
 * A353960  a(1)=1. Thereafter, if a(n) has occurred k (>1) times in a(j), 1
 *          <= j <= n then a(n+1) = k*a(n). If a(n) is a first occurrence,
 *          a(n+1) = A078709(a(n)).
 * https://oeis.org/A353960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; k = number of occurrences of a(n) among a(1..n); a(n+1)
 *            = k*a(n) if k>1, else a(n+1) = A078709(a(n)) =
 *            floor(a(n)/d(a(n))). Computed by direct simulation with a hash
 *            table of counts
 * Range:     u8 n <= 82, u16 n <= 1264, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 9119 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A353960_terms = NULL;
static size_t A353960_len = 0, A353960_capt = 0;
static seq_t *A353960_keys = NULL;
static uint64_t *A353960_cnts = NULL;
static size_t A353960_cap = 0, A353960_used = 0;
static uint64_t A353960_lastk = 0;

static size_t A353960_hash(seq_t x)
{
    unsigned long long r = (unsigned long long)(x % (seq_t)1000000007);
    return (size_t)((r * 2654435761ULL) >> 3);
}

static void A353960_rehash(size_t ncap)
{
    seq_t *nk = mem_new(seq_t, ncap);
    uint64_t *nc = (uint64_t *)calloc(ncap, sizeof(uint64_t));
    for (size_t i = 0; i < A353960_cap; i++) {
        if (A353960_cnts[i]) {
            size_t j = A353960_hash(A353960_keys[i]) & (ncap - 1);
            while (nc[j]) j = (j + 1) & (ncap - 1);
            nk[j] = A353960_keys[i];
            nc[j] = A353960_cnts[i];
        }
    }
    if (A353960_keys) mem_free(A353960_keys);
    if (A353960_cnts) free(A353960_cnts);
    A353960_keys = nk;
    A353960_cnts = nc;
    A353960_cap = ncap;
}

static uint64_t A353960_bump(seq_t x)
{
    if ((A353960_used + 1) * 2 > A353960_cap)
        A353960_rehash(A353960_cap ? A353960_cap * 2 : 1024);
    size_t j = A353960_hash(x) & (A353960_cap - 1);
    while (A353960_cnts[j] && A353960_keys[j] != x) j = (j + 1) & (A353960_cap - 1);
    if (!A353960_cnts[j]) {
        A353960_keys[j] = x;
        A353960_used++;
    }
    return ++A353960_cnts[j];
}

static seq_t A078709_a(seq_t n)
{
    seq_t t = n;
    seq_t d = 1;
    seq_t c = 0;
    while (t % 2 == 0) { t /= 2; c++; }
    d *= (c + 1);
    for (seq_t p = 3; p <= t / p; p += 2) {
        if (t % p == 0) {
            c = 0;
            while (t % p == 0) { t /= p; c++; }
            d *= (c + 1);
        }
    }
    if (t > 1) d *= 2;
    return n / d;
}

static void A353960_push(seq_t v)
{
    if (A353960_len == A353960_capt) {
        size_t nc = A353960_capt ? A353960_capt * 2 : 1024;
        seq_t *nt = mem_new(seq_t, nc);
        for (size_t i = 0; i < A353960_len; i++) nt[i] = A353960_terms[i];
        if (A353960_terms) mem_free(A353960_terms);
        A353960_terms = nt;
        A353960_capt = nc;
    }
    A353960_terms[A353960_len++] = v;
    A353960_lastk = A353960_bump(v);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (A353960_len == 0) A353960_push((seq_t)1);
    while ((idx_t)A353960_len < n) {
        seq_t cur = A353960_terms[A353960_len - 1];
        seq_t nxt;
        if (A353960_lastk > 1)
            nxt = (seq_t)A353960_lastk * cur;
        else
            nxt = A078709_a(cur);
        A353960_push(nxt);
    }
    return A353960_terms[(size_t)(n - 1)];
}
