/*
 * A387613  Numbers k such that A386482(k) and A386482(k+2) are coprime.
 * https://oeis.org/A387613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. Build A386482 with native integers (a(1)=1, a(2)=2; next
 *            term = greatest unused k < prev with gcd(k,prev)>1, otherwise
 *            the smallest unused k > prev with gcd(k,prev)>1, found by
 *            stepping through multiples of each prime factor of prev). List
 *            the k with gcd(A386482(k), A386482(k+2)) = 1
 * Range:     u8 n <= 61, u16 n <= 3251, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9330 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mem.h"

static uint64_t *A387613_s = NULL;
static size_t A387613_len = 0, A387613_cap = 0;
static uint8_t *A387613_used = NULL;
static size_t A387613_ucap = 0;
static uint64_t *A387613_found = NULL;
static size_t A387613_nf = 0, A387613_fcap = 0;
static size_t A387613_nextk = 1;

static uint64_t A387613_gcd(uint64_t x, uint64_t y)
{
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

static void A387613_need_used(size_t need)
{
    if (need < A387613_ucap) return;
    size_t nc = A387613_ucap ? A387613_ucap : 64;
    while (nc <= need) nc <<= 1;
    uint8_t *t = (uint8_t *)realloc(A387613_used, nc);
    if (!t) abort();
    memset(t + A387613_ucap, 0, nc - A387613_ucap);
    A387613_used = t;
    A387613_ucap = nc;
}

static void A387613_push(uint64_t v)
{
    if (A387613_len >= A387613_cap) {
        size_t nc = A387613_cap ? A387613_cap * 2 : 64;
        uint64_t *t = (uint64_t *)realloc(A387613_s, nc * sizeof(uint64_t));
        if (!t) abort();
        A387613_s = t;
        A387613_cap = nc;
    }
    A387613_s[A387613_len++] = v;
    A387613_need_used((size_t)v);
    A387613_used[(size_t)v] = 1;
}

static void A387613_step(void)
{
    uint64_t prev = A387613_s[A387613_len - 1];
    uint64_t primes[64];
    int np = 0;
    uint64_t m = prev;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            primes[np++] = p;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) primes[np++] = m;

    /* greatest unused k < prev sharing a prime factor */
    uint64_t best = 0;
    for (int i = 0; i < np; i++) {
        uint64_t p = primes[i];
        for (uint64_t k = prev - p; k > best && k >= p; k -= p) {
            if (!A387613_used[(size_t)k]) { best = k; break; }
        }
    }
    if (best == 0) {
        /* smallest unused k > prev sharing a prime factor */
        uint64_t lo = UINT64_MAX;
        for (int i = 0; i < np; i++) {
            uint64_t p = primes[i];
            for (uint64_t k = prev + p; k < lo; k += p) {
                A387613_need_used((size_t)k);
                if (!A387613_used[(size_t)k]) { lo = k; break; }
            }
        }
        best = lo;
    }
    A387613_push(best);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (!A387613_s) {
        A387613_need_used(2);
        A387613_push(1);
        A387613_push(2);
    }
    while (A387613_nf < (size_t)n) {
        size_t k = A387613_nextk;
        while (A387613_len < k + 2) A387613_step();
        if (A387613_gcd(A387613_s[k - 1], A387613_s[k + 1]) == 1) {
            if (A387613_nf >= A387613_fcap) {
                size_t nc = A387613_fcap ? A387613_fcap * 2 : 64;
                uint64_t *t = (uint64_t *)realloc(A387613_found, nc * sizeof(uint64_t));
                if (!t) abort();
                A387613_found = t;
                A387613_fcap = nc;
            }
            A387613_found[A387613_nf++] = (uint64_t)k;
        }
        A387613_nextk++;
    }
    return (seq_t)A387613_found[(size_t)n - 1];
}
