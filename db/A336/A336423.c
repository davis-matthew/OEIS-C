/*
 * A336423  Number of strict chains of divisors from n to 1 using terms of
 *          A130091 (numbers with distinct prime multiplicities).
 * https://oeis.org/A336423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{d|n, d<n, d∈S} a(d), a(1)=1 * where S = { numbers with
 *            distinct prime multiplicities } (A130091) * and a(n)=0 for n∉S.
 *            This relation counts strict chains of * divisors from n to 1
 *            using only elements of S
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   exps_distinct, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/exps_distinct.h"

/* ------------------------------------------------------------ */
/* Simple hash table for memoisation (idx_t → seq_t).  Empty key=0. */
/* ------------------------------------------------------------ */
typedef struct {
    idx_t *keys;
    seq_t *vals;
    size_t cap;
    size_t used;
} ht_t;

static ht_t memo = {0};

static void ht_init(void) {
    if (memo.keys) return;
    memo.cap = 64;
    memo.used = 0;
    memo.keys = (idx_t *)calloc(memo.cap, sizeof(idx_t));
    memo.vals = (seq_t *)calloc(memo.cap, sizeof(seq_t));
}

static size_t hash_u64(idx_t x) {
    size_t h = (size_t)x;
    h ^= h >> 33;
    h *= 0xff51afd7ed558ccdULL;
    h ^= h >> 33;
    h *= 0xc4ceb9fe1a85ec53ULL;
    h ^= h >> 33;
    return h & (memo.cap - 1);
}

static void ht_resize(void) {
    size_t new_cap = memo.cap << 1;
    idx_t *new_keys = (idx_t *)calloc(new_cap, sizeof(idx_t));
    seq_t *new_vals = (seq_t *)calloc(new_cap, sizeof(seq_t));
    for (size_t i = 0; i < memo.cap; ++i) {
        idx_t k = memo.keys[i];
        if (!k) continue;
        size_t pos = hash_u64(k) & (new_cap - 1);
        while (new_keys[pos]) pos = (pos + 1) & (new_cap - 1);
        new_keys[pos] = k;
        new_vals[pos] = memo.vals[i];
    }
    free(memo.keys);
    free(memo.vals);
    memo.keys = new_keys;
    memo.vals = new_vals;
    memo.cap = new_cap;
}

static bool ht_find(idx_t key, seq_t *value) {
    if (!memo.keys) return false;
    size_t pos = hash_u64(key) & (memo.cap - 1);
    while (memo.keys[pos]) {
        if (memo.keys[pos] == key) {
            *value = memo.vals[pos];
            return true;
        }
        pos = (pos + 1) & (memo.cap - 1);
    }
    return false;
}

static void ht_insert(idx_t key, seq_t value) {
    if (!memo.keys) ht_init();
    if ((memo.used + 1) * 4 > memo.cap * 3) ht_resize();
    size_t pos = hash_u64(key) & (memo.cap - 1);
    while (memo.keys[pos]) pos = (pos + 1) & (memo.cap - 1);
    memo.keys[pos] = key;
    memo.vals[pos] = value;
    ++memo.used;
}

/* ------------------------------------------------------------ */
/* Helper: test whether exponents are pairwise distinct.       */

/* Forward declaration for recursion                                 */
static seq_t rec(idx_t n);

/* Recursively generate proper divisors of n and sum rec(d).          */
static void dfs_div(uint64_t *pr, uint64_t *ex, size_t cnt,
                    size_t idx, idx_t cur, idx_t base, seq_t *sum) {
    if (idx == cnt) {
        if (cur == base) return;      /* skip the number itself      */
        seq_t val = rec(cur);          /* recursion on divisor       */
        *sum += val;
        return;
    }
    idx_t prod = 1;
    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        idx_t next = cur * prod;
        dfs_div(pr, ex, cnt, idx + 1, next, base, sum);
        prod *= pr[idx];
    }
}

/* Main recursive computation                                        */
static seq_t rec(idx_t n) {
    if (n == 1) return 1;

    seq_t cached;
    if (ht_find(n, &cached)) return cached;

    /* factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* check distinct exponents */
    if (!exps_distinct(exps, cnt)) {
        ht_insert(n, 0);
        return 0;
    }

    /* sum over proper divisors */
    seq_t total = 0;
    dfs_div(primes, exps, cnt, 0, 1, n, &total);

    ht_insert(n, total);
    return total;
}

/* ------------------------------------------------------------ */
/* Public function required by the problem.                      */
/* ------------------------------------------------------------ */
seq_t a(idx_t n) {
    return rec(n);
}
