/*
 * A379033  Numbers that are the product of exactly three (not necessarily
 *          distinct) primes and these primes are sides of a nondegenerate
 *          triangle.
 * https://oeis.org/A379033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   enumeration of products of exactly three primes (not necessarily
 *            distinct) * that satisfy the triangle inequality p+q>r (with
 *            p≤q≤r). The function * memoizes the list so that any n ≥ 1 can
 *            be answered efficiently
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   upper_bound_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/upper_bound_idx.h"

/* dynamic array for seq_t values */
typedef struct {
    seq_t *data;
    size_t size;
    size_t cap;
} dynarr_t;

static void dynarr_init(dynarr_t *a) {
    a->data = NULL;
    a->size = 0;
    a->cap  = 0;
}
static void dynarr_free(dynarr_t *a) {
    free(a->data);
    a->data = NULL;
    a->size = 0;
    a->cap  = 0;
}
static void dynarr_push(dynarr_t *a, seq_t v) {
    if (a->size == a->cap) {
        size_t newcap = (a->cap == 0) ? 16 : a->cap * 2;
        seq_t *tmp = (seq_t *)realloc(a->data, newcap * sizeof(seq_t));
        if (!tmp) exit(1);
        a->data = tmp;
        a->cap  = newcap;
    }
    a->data[a->size++] = v;
}

/* binary search helpers */
static size_t lower_bound_idx(const uint32_t *a, size_t n, uint32_t val) {
    size_t l = 0, r = n;
    while (l < r) {
        size_t m = (l + r) >> 1;
        if (a[m] < val) l = m + 1;
        else r = m;
    }
    return l;
}

/* quicksort comparator for seq_t */
static int cmp_seq_t(const void *p1, const void *p2) {
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* Sieve to generate primes up to limit */
static void generate_primes(uint32_t limit, uint32_t **primes_out, size_t *count_out) {
    if (limit < 2) {
        *primes_out = NULL;
        *count_out = 0;
        return;
    }
    char *is_prime = (char *)malloc((size_t)limit + 1);
    if (!is_prime) exit(1);
    memset(is_prime, 1, (size_t)limit + 1);
    is_prime[0] = is_prime[1] = 0;
    for (uint32_t i = 2; (uint64_t)i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (uint64_t j = (uint64_t)i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }
    size_t cnt = 0;
    for (uint32_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;
    uint32_t *pr = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!pr) exit(1);
    size_t idx = 0;
    for (uint32_t i = 2; i <= limit; ++i)
        if (is_prime[i]) pr[idx++] = i;
    free(is_prime);
    *primes_out = pr;
    *count_out  = cnt;
}

/* Global storage for sequence */
static dynarr_t seq_arr = {NULL, 0, 0};
static uint64_t current_bound = 0;

/* Regenerate the sequence up to the given bound */
static void recompute_seq(uint64_t bound) {
    uint32_t limit_prime = (uint32_t)(bound / 4) + 1;
    uint32_t *primes;
    size_t prime_count;
    generate_primes(limit_prime, &primes, &prime_count);

    dynarr_t tmp;
    dynarr_init(&tmp);

    for (size_t ir = 0; ir < prime_count; ++ir) {
        uint32_t r = primes[ir];
        if ((uint64_t)r > bound / 4) break;               /* 4*r <= bound */

        uint64_t qmax_val = r;
        uint64_t tmpq = bound / ((uint64_t)2 * r);
        if (tmpq < qmax_val) qmax_val = tmpq;

        size_t iq_end_all = upper_bound_idx(primes, prime_count, (uint32_t)qmax_val);
        if (iq_end_all == 0) continue;
        size_t iq_end = (iq_end_all <= ir) ? (iq_end_all - 1) : ir;

        for (size_t iq = 0; iq <= iq_end; ++iq) {
            uint32_t q = primes[iq];

            uint32_t needed = 2;
            if (r > q) needed = r - q + 1;

            uint64_t prod_limit = bound / ((uint64_t)q * (uint64_t)r);
            uint32_t maxp_val = q;
            if (prod_limit < maxp_val) maxp_val = (uint32_t)prod_limit;

            size_t p_start = lower_bound_idx(primes, prime_count, needed);
            if (p_start >= prime_count) continue;
            if (primes[p_start] < needed) ++p_start;
            if (p_start >= prime_count) continue;

            size_t p_end_all = upper_bound_idx(primes, prime_count, maxp_val);
            if (p_end_all == 0) continue;
            size_t p_end = p_end_all - 1;

            if (p_start > p_end) continue;

            for (size_t ip = p_start; ip <= p_end; ++ip) {
                uint32_t p = primes[ip];
                seq_t prod = (seq_t)p * (seq_t)q * (seq_t)r;
                dynarr_push(&tmp, prod);
            }
        }
    }

    qsort(tmp.data, tmp.size, sizeof(seq_t), cmp_seq_t);

    dynarr_free(&seq_arr);
    seq_arr = tmp;

    free(primes);
}

/* Main access function */
seq_t a(idx_t n) {
    if (!seq_arr.data) {
        current_bound = 100000000ULL;      /* initial breadth */
        recompute_seq(current_bound);
    }
    while (seq_arr.size < (size_t)n) {
        current_bound *= 2;
        recompute_seq(current_bound);
    }
    return seq_arr.data[n - 1];
}
