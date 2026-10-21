/*
 * A299701  Number of distinct subset-sums of the integer partition with Heinz
 *          number n.
 * https://oeis.org/A299701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct subset sums of the multiset of indices
 *            of the prime factors of n (counting multiplicities). Computed by
 *            sieving primes up to n, then a DP that enumerates all reachable
 *            sums
 * Range:     u8 n <= 9754, u16 n <= 9754, u32 n <= 9754, u64 n <= 9754,
 *            u128 n <= 9754, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Prime sieve: globals for memoisation across calls */
static uint32_t *g_primes = NULL;   /* array of primes */
static size_t g_primes_cnt = 0;     /* number of primes in g_primes */
static idx_t   g_primes_limit = 0;  /* largest number sieved up to */

/* Ensure primes up to 'limit' are available */
static void ensure_primes(idx_t limit) {
    if (limit <= g_primes_limit && g_primes != NULL)
        return;                     /* already sufficient */

    /* free previous list if any */
    free(g_primes);
    g_primes = NULL;
    g_primes_cnt = 0;
    g_primes_limit = 0;

    size_t n = (size_t)limit;
    bool *is_prime = (bool*)calloc(n + 1, sizeof(bool));
    if (is_prime == NULL)
        return;                     /* allocation failure – will use none */
    for (size_t i = 2; i <= n; ++i)
        is_prime[i] = true;
    for (size_t p = 2; p * p <= n; ++p) {
        if (is_prime[p]) {
            for (size_t mult = p * p; mult <= n; mult += p)
                is_prime[mult] = false;
        }
    }
    size_t cnt = 0;
    for (size_t i = 2; i <= n; ++i)
        if (is_prime[i]) ++cnt;

    g_primes = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    if (g_primes == NULL) {
        free(is_prime);
        return;                     /* allocation failure – will use none */
    }
    size_t idx = 0;
    for (size_t i = 2; i <= n; ++i)
        if (is_prime[i]) g_primes[idx++] = (uint32_t)i;

    g_primes_cnt = cnt;
    g_primes_limit = limit;
    free(is_prime);
}

/* Binary search: index of prime 'p' (1‑based). */
static idx_t prime_index(uint64_t p) {
    size_t lo = 0, hi = g_primes_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        uint64_t val = g_primes[mid];
        if (val == p)
            return (idx_t)(mid + 1);
        else if (val < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return 0;  /* should not happen for valid primes */
}

typedef struct {
    idx_t part;  /* index of the prime */
    idx_t exp;   /* exponent (multiplicity) */
} PartExp;

/* Main sequence function */
seq_t a(idx_t n) {
    if (n < 1) return 0;           /* undefined for n=0, but safe */

    /* Ensure primes up to n for factorisation and index lookup */
    if (n > g_primes_limit)
        ensure_primes(n);

    /* Factorise n */
    idx_t tmp = n;
    PartExp *vec = NULL;
    size_t vec_size = 0, vec_cap = 0;
    idx_t sum_parts = 0;           /* total sum of indices with multiplicity */

    for (size_t i = 0; i < g_primes_cnt; ++i) {
        uint32_t p = g_primes[i];
        if ((uint64_t)p * (uint64_t)p > (uint64_t)tmp)
            break;
        if (tmp % p == 0) {
            idx_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            idx_t part = (idx_t)(i + 1);   /* prime(i+1) = p */
            if (vec_size == vec_cap) {
                size_t new_cap = vec_cap ? vec_cap * 2 : 8;
                PartExp *new_vec = (PartExp*)malloc(new_cap * sizeof(PartExp));
                if (new_vec == NULL) { free(vec); return 0; }
                if (vec_size)
                    memcpy(new_vec, vec, vec_size * sizeof(PartExp));
                free(vec);
                vec = new_vec;
                vec_cap = new_cap;
            }
            vec[vec_size].part = part;
            vec[vec_size].exp  = e;
            ++vec_size;
            sum_parts += part * e;
        }
    }
    if (tmp > 1) {   /* remaining prime factor > sqrt(original n) */
        uint64_t p = tmp;
        idx_t part = prime_index(p);
        if (part == 0) {         /* p > current sieve limit */
            ensure_primes(p);
            part = prime_index(p);
        }
        if (vec_size == vec_cap) {
            size_t new_cap = vec_cap ? vec_cap * 2 : 8;
            PartExp *new_vec = (PartExp*)malloc(new_cap * sizeof(PartExp));
            if (new_vec == NULL) { free(vec); return 0; }
            if (vec_size)
                memcpy(new_vec, vec, vec_size * sizeof(PartExp));
            free(vec);
            vec = new_vec;
            vec_cap = new_cap;
        }
        vec[vec_size].part = part;
        vec[vec_size].exp  = 1;
        ++vec_size;
        sum_parts += part;
    }

    /* DP: bitset of reachable sums up to sum_parts */
    size_t sz_dp = (size_t)(sum_parts + 1);
    bool *dp = (bool*)malloc(sz_dp * sizeof(bool));
    if (dp == NULL) { free(vec); return 0; }
    memset(dp, 0, sz_dp * sizeof(bool));
    dp[0] = true;

    for (size_t k = 0; k < vec_size; ++k) {
        idx_t weight = vec[k].part;
        idx_t exponent = vec[k].exp;
        for (idx_t t = 0; t < exponent; ++t) {
            for (idx_t s = sum_parts; s >= weight; --s) {
                if (dp[s - weight])
                    dp[s] = true;
            }
        }
    }

    /* Count distinct sums (including 0) */
    seq_t count = 0;
    for (idx_t s = 0; s <= sum_parts; ++s)
        if (dp[s]) ++count;

    free(dp);
    free(vec);

    return count;
}
