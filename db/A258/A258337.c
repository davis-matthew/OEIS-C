/*
 * A258337  Smallest prime starting with n that does not appear earlier.
 * https://oeis.org/A258337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest prime starting with decimal string of n that has
 *            not appeared as a(k) for any k < n. Sequence is a permutation of
 *            all primes
 * Range:     u8 n <= 19, u16 n <= 661, u32 n <= 360, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;          /* cache[n] = a(n), 1‑based */
static size_t cache_cap = 0;         /* allocated size of cache */
static size_t cache_len = 0;         /* maximum index computed so far */

static seq_t *used_table = NULL;    /* hash table of primes already used */
static size_t used_size = 0;        /* size of hash table (power of two) */
static size_t used_cnt  = 0;        /* number of entries */

/* --------------------------------- power of ten --------------------------------- */
static seq_t pow10[20];
static bool pow10_init = false;
static void init_pow10(void) {
    if (pow10_init) return;
    pow10[0] = (seq_t)1;
    for (int i = 1; i < 20; ++i) pow10[i] = pow10[i - 1] * (seq_t)10;
    pow10_init = true;
}

/* ---------------------------- used hash table helpers ---------------------------- */
static void used_init(void) {
    used_size = 128;
    used_table = (seq_t*)malloc(used_size * sizeof(seq_t));
    for (size_t i = 0; i < used_size; ++i) used_table[i] = (seq_t)0;
}

static void used_resize(void) {
    size_t new_size = used_size * 2;
    seq_t *new_table = (seq_t*)malloc(new_size * sizeof(seq_t));
    for (size_t i = 0; i < new_size; ++i) new_table[i] = (seq_t)0;

    for (size_t i = 0; i < used_size; ++i) {
        seq_t v = used_table[i];
        if (v) {
            size_t idx = (size_t)(v % (seq_t)new_size);
            while (new_table[idx] != (seq_t)0) idx = (idx + 1) % new_size;
            new_table[idx] = v;
        }
    }
    free(used_table);
    used_table = new_table;
    used_size  = new_size;
}

static bool used_contains(seq_t v) {
    if (!used_table) return false;                 /* nothing used yet */
    size_t idx = (size_t)(v % (seq_t)used_size);
    while (true) {
        seq_t cur = used_table[idx];
        if (cur == (seq_t)0) return false;
        if (cur == v)        return true;
        idx = (idx + 1) % used_size;
    }
}

static void used_insert(seq_t v) {
    if (!used_table) used_init();
    if (used_cnt >= used_size / 2) used_resize();
    size_t idx = (size_t)(v % (seq_t)used_size);
    while (used_table[idx] != (seq_t)0 && used_table[idx] != v)
        idx = (idx + 1) % used_size;
    if (used_table[idx] == (seq_t)0) {
        used_table[idx] = v;
        ++used_cnt;
    }
}

/* ------------------------------ cache helpers --------------------------------- */
static void cache_prepare(idx_t n) {
    size_t need = (size_t)n + 1;                /* need indices up to n */
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        if (new_cap < need) new_cap = need;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        for (size_t i = cache_cap; i < new_cap; ++i) cache[i] = (seq_t)0;
        cache_cap = new_cap;
    }
}

/* --------------------------- compute sequence up to n --------------------------- */
static void compute_up_to(idx_t n) {
    init_pow10();
    cache_prepare(n);

    for (idx_t k = (idx_t)(cache_len + 1); k <= n; ++k) {
        seq_t answer = 0;
        int m = 0;
        bool found = false;
        while (!found) {
            seq_t lower = (seq_t)k * pow10[m];
            seq_t upper = lower + pow10[m] - (seq_t)1;
            for (seq_t v = lower; v <= upper; ++v) {
                if (!used_contains(v) && is_prime_big(v)) {
                    answer = v;
                    found = true;
                    break;
                }
                /* beware of overflow when v jumps past upper; this loop stops at upper */
            }
            ++m;
        }
        cache[(size_t)k] = answer;
        used_insert(answer);
    }
    cache_len = (size_t)n;
}

/* ------------------------------- public function ------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;        /* undefined, but safe */
    compute_up_to(n);
    return cache[(size_t)n];
}
