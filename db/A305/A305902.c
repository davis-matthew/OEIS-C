/*
 * A305902  Restricted growth sequence transform of A305900(A048673(n)).
 * https://oeis.org/A305902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence transform of f(n) =
 *            A305900(A048673(n)), where A305900(1) = 1, A305900(p) = 0 for
 *            primes p > 3, A305900(k) = k otherwise; computed by assigning
 *            each new f-value the next unused label, scanning n = 1, 2,
 * Range:     u8 n <= 322, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 115 terms through n = 8664 (2026-11-08)
 * Helpers:   factor_u64, is_prime_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"
#include "../utils/is_prime_u64.h"

/* A048673(n) = (A003961(n)+1)/2 computed in uint64 (fine for the needed range). */
static uint64_t A048673_u64(uint64_t n)
{
    if (n == 1) return 1;
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);
    uint64_t prod = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t np = next_prime(primes[i]);
        for (uint64_t k = 0; k < exps[i]; ++k) prod *= np;
    }
    return (prod + 1) / 2;
}

static uint64_t A305902_key(uint64_t n)
{
    uint64_t v = A048673_u64(n);
    if (v > 3 && is_prime_u64(v)) return 0;
    return v;
}

/* memo of computed terms */
static uint64_t *A305902_vals = NULL;   /* vals[i] for i = 1..computed */
static size_t A305902_computed = 0;
static size_t A305902_valcap = 0;

/* hash table: key -> label */
static uint64_t *A305902_hkeys = NULL;
static uint64_t *A305902_hlab = NULL;   /* 0 = empty slot */
static size_t A305902_hcap = 0;
static size_t A305902_hcount = 0;
static uint64_t A305902_nextlabel = 1;

static size_t A305902_hash(uint64_t k, size_t cap)
{
    k ^= k >> 33;
    k *= 0xff51afd7ed558ccdULL;
    k ^= k >> 33;
    return (size_t)(k & (uint64_t)(cap - 1));
}

static void A305902_hinsert_raw(uint64_t *keys, uint64_t *labs, size_t cap,
                                uint64_t k, uint64_t lab)
{
    size_t h = A305902_hash(k, cap);
    while (labs[h] != 0) h = (h + 1) & (cap - 1);
    keys[h] = k;
    labs[h] = lab;
}

static void A305902_hgrow(void)
{
    size_t ncap = A305902_hcap ? A305902_hcap * 2 : 1024;
    uint64_t *nk = (uint64_t *)calloc(ncap, sizeof(uint64_t));
    uint64_t *nl = (uint64_t *)calloc(ncap, sizeof(uint64_t));
    for (size_t i = 0; i < A305902_hcap; ++i)
        if (A305902_hlab[i] != 0)
            A305902_hinsert_raw(nk, nl, ncap, A305902_hkeys[i], A305902_hlab[i]);
    free(A305902_hkeys);
    free(A305902_hlab);
    A305902_hkeys = nk;
    A305902_hlab = nl;
    A305902_hcap = ncap;
}

static uint64_t A305902_label(uint64_t k)
{
    if ((A305902_hcount + 1) * 2 > A305902_hcap) A305902_hgrow();
    size_t h = A305902_hash(k, A305902_hcap);
    while (A305902_hlab[h] != 0) {
        if (A305902_hkeys[h] == k) return A305902_hlab[h];
        h = (h + 1) & (A305902_hcap - 1);
    }
    A305902_hkeys[h] = k;
    A305902_hlab[h] = A305902_nextlabel++;
    A305902_hcount++;
    return A305902_hlab[h];
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if ((size_t)n > A305902_computed) {
        if ((size_t)n + 1 > A305902_valcap) {
            size_t ncap = A305902_valcap ? A305902_valcap : 1024;
            while (ncap < (size_t)n + 1) ncap *= 2;
            uint64_t *nv = (uint64_t *)realloc(A305902_vals, ncap * sizeof(uint64_t));
            A305902_vals = nv;
            A305902_valcap = ncap;
        }
        for (size_t i = A305902_computed + 1; i <= (size_t)n; ++i)
            A305902_vals[i] = A305902_label(A305902_key((uint64_t)i));
        A305902_computed = (size_t)n;
    }
    return (seq_t)A305902_vals[(size_t)n];
}
