/*
 * A300235  Restricted growth sequence transform of A291765, filter combining
 *          A001065(n) and A046523(n), the sum of proper divisors and the
 *          prime signature of n.
 * https://oeis.org/A300235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = label of the pair (σ(n)-n, prime exponents of n sorted
 *            ascending) in the order of first appearance, starting with
 *            (0,∅)=1
 * Range:     u8 n <= 355, u16 n <= 9893, u32 n <= 9893, u64 n <= 9893,
 *            u128 n <= 9893, big any n
 * Helpers:   pack_exponents
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/pack_exponents.h"

/* ------------------------------------------------------------------------- */
/*  Helpers for smallest‑prime‑factor sieve                                  */
/* ------------------------------------------------------------------------- */
static uint64_t *spf = NULL;          /* spf[i] = smallest prime factor of i */
static size_t   spf_cap = 0;          /* size of the spf array */

static void spf_build(size_t limit)
{
    if (limit + 1 <= spf_cap) return;                /* already built */

    size_t new_cap = limit + 1;
    uint64_t *tmp = (uint64_t *)realloc(spf, new_cap * sizeof(uint64_t));
    if (!tmp) exit(1);
    spf = tmp;
    spf_cap = new_cap;

    for (size_t i = 0; i < new_cap; ++i) spf[i] = i;
    for (size_t i = 2; i * i < new_cap; ++i) {
        if (spf[i] == i) {                          /* i is prime */
            for (size_t j = i * i; j < new_cap; j += i)
                if (spf[j] == j) spf[j] = i;
        }
    }
}

/* ------------------------------------------------------------------------- */
/*  Key (σ(n)-n , prime signature) hash table                                */
/* ------------------------------------------------------------------------- */
typedef struct {
    uint64_t sum;    /* σ(n)-n */
    uint64_t sig;    /* packed prime exponents (4 bits each) */
} Key;

#define HASH_BITS   16
#define HASH_SIZE   (1U << HASH_BITS)
#define HASH_MASK   (HASH_SIZE - 1)
static Key     *hash_keys  = NULL;
static seq_t   *hash_vals  = NULL;   /* id of key */
static seq_t    next_id     = (seq_t)2;  /* id used for new keys (1 already used) */
static bool     hash_initd   = false;

static void hash_init(void)
{
    if (hash_initd) return;
    hash_keys = (Key *)calloc(HASH_SIZE, sizeof(Key));
    hash_vals = (seq_t *)calloc(HASH_SIZE, sizeof(seq_t));
    if (!hash_keys || !hash_vals) exit(1);
    /* mark all slots as empty */
    for (size_t i = 0; i < HASH_SIZE; ++i)
        hash_keys[i].sum = UINT64_MAX;

    /* insert the first key (0,∅) with id 1 */
    size_t pos = ((size_t)0 ^ (size_t)0 * 0x9e3779b97f4a7c15ULL) & HASH_MASK;
    hash_keys[pos].sum = 0;
    hash_keys[pos].sig = 0;
    hash_vals[pos]     = (seq_t)1;
    hash_initd = true;
}

static inline size_t hash_position(uint64_t sum, uint64_t sig)
{
    return ((size_t)(sum ^ (sig << 1)) * 0x9e3779b97f4a7c15ULL) & HASH_MASK;
}

static seq_t hash_find_or_insert(uint64_t sum, uint64_t sig)
{
    size_t pos = hash_position(sum, sig);
    while (1) {
        if (hash_keys[pos].sum == UINT64_MAX) {
            /* empty slot – insert */
            hash_keys[pos].sum = sum;
            hash_keys[pos].sig = sig;
            hash_vals[pos]     = next_id;
            return next_id++;
        }
        if (hash_keys[pos].sum == sum && hash_keys[pos].sig == sig)
            return hash_vals[pos];
        pos = (pos + 1) & HASH_MASK;
    }
}

/* ------------------------------------------------------------------------- */
/*  Cache of a(n) values                                                     */
/* ------------------------------------------------------------------------- */
static seq_t *cached     = NULL;
static size_t cache_cap  = 0;   /* allocated size (>= n+1) */
static size_t cache_len  = 0;   /* largest index computed */

static void cache_ensure(size_t n)
{
    if (cache_cap >= n + 1) return;
    size_t new_cap = cache_cap == 0 ? 64 : cache_cap;
    while (new_cap <= n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cached, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    cached = tmp;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------------- */
/*  Pack prime exponents (4 bits each)                                       */

/* ------------------------------------------------------------------------- */
/*  Main sequence function                                                  */
/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */

    size_t N = (size_t)n;

    /* ensure caches large enough */
    cache_ensure(N);

    /* prepare smallest‑prime‑factor table */
    spf_build(N);

    /* initialise hash table once */
    if (!hash_initd) hash_init();

    /* compute missing terms up to N */
    for (size_t i = cache_len + 1; i <= N; ++i) {
        /* factor i using SPF */
        uint64_t temp = i;
        seq_t sigma = (seq_t)1;
        int exps_arr[16];
        size_t exps_cnt = 0;

        while (temp > 1) {
            uint64_t p = spf[temp];
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            /* term = 1 + p + p^2 + ... + p^e */
            seq_t term = (seq_t)1;
            seq_t pow  = (seq_t)p;
            for (int k = 1; k <= e; ++k) {
                term += pow;
                pow  *= (seq_t)p;
            }
            sigma *= term;
            exps_arr[exps_cnt++] = e;
        }
        seq_t prop_sum = sigma - (seq_t)i;   /* sum of proper divisors */

        /* sort exponents ascending (small count) */
        for (size_t a = 0; a < exps_cnt; ++a)
            for (size_t b = a + 1; b < exps_cnt; ++b)
                if (exps_arr[a] > exps_arr[b]) {
                    int tmp = exps_arr[a];
                    exps_arr[a] = exps_arr[b];
                    exps_arr[b] = tmp;
                }

        uint64_t sig_key = pack_exponents(exps_arr, exps_cnt);
        uint64_t sum_key = (uint64_t)prop_sum;

        seq_t id = hash_find_or_insert(sum_key, sig_key);
        cached[i] = id;
    }

    cache_len = N;
    return cached[N];
}
