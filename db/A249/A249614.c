/*
 * A249614  Integers that reach the (47360, 29127) cycle described in A234534,
 *          after iterations of numerator(sigma(n)/n) = A017665(n).
 * https://oeis.org/A249614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 23, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   factor_u64, hash_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A249614 – integers that reach the (47360,29127) cycle
 * by iterating f(n) = numerator(σ(n)/n).
 * The function a(idx_t n) returns the nth term (n≥1).
 * It is implemented by a linear search with memoisation of the
 * reachability test.  The search is O(max_term), which is
 * fast enough for the required limits.  All arithmetic is done
 * with 64 bit integers; arbitrary‑precision support for seq_t
 * is handled by simple casts that work for built‑in types and
 * for typical big integer classes used in the build. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Helper for integer factorisation */
#include "../utils/factor_u64.h"
#include "../utils/hash_uint64.h"

/* ---- Small utility: integer GCD (unsigned long long) */
static inline unsigned long long gcd64(unsigned long long a,
                                       unsigned long long b) {
    while (b) {
        unsigned long long t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* ---- Compute f(n) = numerator(σ(n)/n) for 64 bit n */
static inline unsigned long long f_uint64(unsigned long long n) {
    if (n == 0) return 0;

    /* factor n */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);    /* primes[i]^exps[i] = n */

    /* compute σ(n) as product over primes:
       σ(p^e) = (p^(e+1)-1)/(p-1)  */
    __uint128_t sigma = 1;
    for (size_t i = 0; i < cnt; ++i) {
        unsigned long long p = primes[i];
        unsigned long long e = exps[i];

        __uint128_t num = 1;
        for (unsigned long long j = 0; j < e + 1; ++j)
            num *= p;
        num -= 1;

        __uint128_t den = p - 1;
        __uint128_t term = num / den;      /* division exact */

        sigma *= term;
    }

    /* reduce σ(n)/n and return numerator */
    unsigned long long sigma64 = (unsigned long long)sigma;
    unsigned long long g = gcd64(sigma64, n);
    return sigma64 / g;
}

/* ---- Memoisation of reachability results ---- */
struct memo_entry {
    uint64_t key;
    uint8_t val;   /* 0 = empty, 1 = true, 2 = false */
};

static size_t memo_cap        = 1 << 20;   /* start with 1 048 576 slots */
static struct memo_entry *memo_tbl = NULL;
static size_t memo_used     = 0;

/* initialise the table */
static void init_memo(void) {
    if (memo_tbl) return;
    memo_tbl = (struct memo_entry *)calloc(memo_cap, sizeof(struct memo_entry));
}

/* lookup */
static uint8_t memo_get(uint64_t key, bool *out) {
    if (!memo_tbl) return 0;
    uint64_t h = hash_uint64(key);
    size_t idx = h & (memo_cap - 1);
    while (true) {
        if (memo_tbl[idx].val == 0) return 0;          /* not found */
        if (memo_tbl[idx].key == key) {
            *out = memo_tbl[idx].val == 1;
            return 1;
        }
        idx = (idx + 1) & (memo_cap - 1);
    }
}

/* store */
static void memo_set(uint64_t key, bool val) {
    init_memo();

    /* resize if load factor > 70 % */
    if ((memo_used + 1) * 1000 / memo_cap > 700) {
        size_t new_cap = memo_cap << 1;
        struct memo_entry *new_tbl =
            (struct memo_entry *)calloc(new_cap, sizeof(struct memo_entry));
        for (size_t i = 0; i < memo_cap; ++i) {
            if (memo_tbl[i].val) {
                uint64_t h = hash_uint64(memo_tbl[i].key);
                size_t idx = h & (new_cap - 1);
                while (new_tbl[idx].val) idx = (idx + 1) & (new_cap - 1);
                new_tbl[idx] = memo_tbl[i];
            }
        }
        free(memo_tbl);
        memo_tbl = new_tbl;
        memo_cap = new_cap;
    }

    uint64_t h = hash_uint64(key);
    size_t idx = h & (memo_cap - 1);
    while (memo_tbl[idx].val) idx = (idx + 1) & (memo_cap - 1);
    memo_tbl[idx].key = key;
    memo_tbl[idx].val = val ? 1 : 2;
    ++memo_used;
}

/* ---- Reachability test ---- */
static bool reaches_cycle(uint64_t n) {
    if (n == 29127ULL || n == 47360ULL) return true;

    bool memo_val;
    if (memo_get(n, &memo_val)) return memo_val;

    /* Path recording – enough space for the typical chain */
    uint64_t path[1024];
    size_t path_len = 0;
    uint64_t cur = n;

    while (true) {
        /* Known terminal */
        if (cur == 29127ULL || cur == 47360ULL) {
            for (size_t i = 0; i < path_len; ++i) memo_set(path[i], true);
            memo_set(n, true);
            return true;
        }
        /* Already known */
        if (memo_get(cur, &memo_val)) {
            for (size_t i = 0; i < path_len; ++i) memo_set(path[i], memo_val);
            memo_set(n, memo_val);
            return memo_val;
        }
        /* Chain too long – fall back to false */
        if (path_len >= 1023) break;

        path[path_len++] = cur;
        cur = f_uint64(cur);
    }
    for (size_t i = 0; i < path_len; ++i) memo_set(path[i], false);
    memo_set(n, false);
    return false;
}

/* ---- Storage for the sequence terms ---- */
static seq_t *hit_terms = NULL;
static idx_t hit_size  = 0;
static idx_t hit_cap   = 0;

/* ensure capacity */
static void hit_grow(idx_t needed) {
    if (needed <= hit_cap) return;
    idx_t new_cap = hit_cap ? hit_cap * 2 : 64;
    while (new_cap < needed) new_cap <<= 1;
    hit_terms = (seq_t *)realloc(hit_terms, new_cap * sizeof(seq_t));
    hit_cap   = new_cap;
}

/* ---- Main sequence function ---- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* safety guard, not used */

    /* generate terms up to the desired index */
    static uint64_t tested_max = 0;
    while (hit_size < n) {
        ++tested_max;
        if (reaches_cycle(tested_max)) {
            hit_grow(hit_size + 1);
            hit_terms[hit_size++] = (seq_t)tested_max;
        }
    }
    return hit_terms[n - 1];
}
