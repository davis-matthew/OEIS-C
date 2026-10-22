/*
 * A331024  Denominator: factorizations divided by strict factorizations
 *          A001055(n)/A045778(n).
 * https://oeis.org/A331024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 9140, u16 n <= 9140, u32 n <= 9140, u64 n <= 9140,
 *            u128 n <= 9140, big any n
 * Helpers:   cmp_uint32, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A331024:  a(n) = A045778(n) / gcd(A001055(n), A045778(n))
 *
 *   A001055(n) = number of factorizations of n (non‑decreasing factors >1)
 *   A045778(n) = number of strict factorizations of n (all factors distinct)
 *
 * The function implements a memoised recursion for the two counts and
 * returns the reduced denominator.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"   /* seq_t gcd(seq_t, seq_t) */
#include "../utils/cmp_uint32.h"

/* ------------------------------------------------------------------ */
/*  Divisor tables for quick access to all d > 1 with d | n          */
/* ------------------------------------------------------------------ */
typedef struct {
    size_t len;
    uint32_t *arr;        /* sorted list of divisors > 1 */
} DivVec;

static DivVec **divvecs      = NULL;      /* dynamic array of pointers   */
static size_t    divvec_cap  = 0;          /* current capacity             */

static void ensure_divvec_capacity(size_t n)
{
    if (n < divvec_cap) return;
    size_t new_cap = divvec_cap ? divvec_cap * 2 : 16;
    while (new_cap <= n) new_cap <<= 1;
    divvecs = (DivVec **)realloc(divvecs, new_cap * sizeof(DivVec *));
    if (!divvecs) abort();
    for (size_t i = divvec_cap; i < new_cap; ++i) divvecs[i] = NULL;
    divvec_cap = new_cap;
}

static void init_divisors_for_n(uint64_t n)
{
    if (n >= divvec_cap) ensure_divvec_capacity(n);
    if (divvecs[n]) return;            /* already built */

    DivVec *dv = (DivVec *)malloc(sizeof(DivVec));
    if (!dv) abort();
    dv->len = 0;
    dv->arr = NULL;
    size_t capacity = 0;

    for (uint64_t d = 2; d * d <= n; ++d) {
        if (n % d == 0) {
            if (dv->len == capacity) {
                capacity = capacity ? capacity << 1 : 4;
                dv->arr = (uint32_t *)realloc(dv->arr, capacity * sizeof(uint32_t));
                if (!dv->arr) abort();
            }
            dv->arr[dv->len++] = (uint32_t)d;
            uint64_t o = n / d;
            if (o != d) {
                if (dv->len == capacity) {
                    capacity = capacity ? capacity << 1 : 4;
                    dv->arr = (uint32_t *)realloc(dv->arr, capacity * sizeof(uint32_t));
                    if (!dv->arr) abort();
                }
                dv->arr[dv->len++] = (uint32_t)o;
            }
        }
    }
    if (dv->len) {
        qsort(dv->arr, dv->len, sizeof(uint32_t), cmp_uint32);
        dv->arr = (uint32_t *)realloc(dv->arr, dv->len * sizeof(uint32_t));
    }
    divvecs[(size_t)n] = dv;
}

/* ------------------------------------------------------------------ */
/*  Memoisation of factorisation counts                               */
/* ------------------------------------------------------------------ */
#define HASH_SIZE (1U << 18)          /* 262144 buckets */

typedef struct CacheEntry {
    uint64_t       n;
    uint64_t       min;
    seq_t          val;
    struct CacheEntry *next;
} CacheEntry;

static CacheEntry *nondec_cache[HASH_SIZE] = {0};
static CacheEntry *strict_cache[HASH_SIZE] = {0};

static size_t hash_key(uint64_t n, uint64_t min)
{
    return (size_t)((n * 1315423911U) ^ (min << 5)) & (HASH_SIZE - 1);
}

static seq_t lookup_cache(CacheEntry **table, uint64_t n, uint64_t min)
{
    size_t h = hash_key(n, min);
    for (CacheEntry *e = table[h]; e; e = e->next) {
        if (e->n == n && e->min == min) return e->val;
    }
    return 0;   /* sentinel indicates not found */
}

static void store_cache(CacheEntry **table, uint64_t n, uint64_t min, seq_t val)
{
    size_t h = hash_key(n, min);
    CacheEntry *e = (CacheEntry *)malloc(sizeof(CacheEntry));
    if (!e) abort();
    e->n = n;
    e->min = min;
    e->val = val;
    e->next = table[h];
    table[h] = e;
}

/* ------------------------------------------------------------------ */
/*  Recursive count of factorizations                                 */
/*  strict == 0  => non‑decreasing (allow equals)                     */
/*  strict == 1  => strictly increasing (no equals)                  */
/* ------------------------------------------------------------------ */
static seq_t count_fact_rec(uint64_t n, uint64_t min, int strict)
{
    if (n < min) return 0;                    /* no valid factorisations */

    CacheEntry **table = strict ? strict_cache : nondec_cache;
    seq_t cached = lookup_cache(table, n, min);
    if (cached) return cached;

    init_divisors_for_n(n);
    DivVec *dv = divvecs[(size_t)n];

    /* base: the factorisation consisting solely of n itself */
    seq_t res = (strict && n <= min) ? 0 : 1;

    if (dv && dv->len) {
        for (size_t i = 0; i < dv->len; ++i) {
            uint64_t d = dv->arr[i];
            if (strict) {
                if (d <= min) continue;          /* need strictly larger */
            } else {
                if (d <  min) continue;          /* non‑decreasing   */
            }
            if (d > n / d) break;                /* keep ordering    */
            res += count_fact_rec(n / d, d, strict);
        }
    }
    store_cache(table, n, min, res);
    return res;
}

static seq_t count_nondec(uint64_t n) { return count_fact_rec(n, 2ULL, 0); }
static seq_t count_strict(uint64_t n) { return count_fact_rec(n, 1ULL, 1); }

/* ------------------------------------------------------------------ */
/*  Main sequence function                                             */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* undefined for n < 1, not used */
    if (n == 1) return 1;               /* a(1) = 1 by definition   */

    uint64_t u = (uint64_t)n;
    seq_t strict_cnt = count_strict(u);
    seq_t nondec_cnt = count_nondec(u);
    seq_t g = gcd(nondec_cnt, strict_cnt);
    return strict_cnt / g;
}
