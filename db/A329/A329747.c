/*
 * A329747  Runs-resistance of the sequence of prime indices of n.
 * https://oeis.org/A329747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9783, u16 n <= 9783, u32 n <= 9783, u64 n <= 9783,
 *            u128 n <= 9783, big any n
 * Helpers:   u64_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A329747 – runs‑resistance of the sequence of prime indices of n
   (number of run‑length transforms until a singleton sequence is reached). */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/u64_sqrt.h"

/* ------------------------------------------------------------------ */
/* Prime sieve (dynamic) – 1‑based indices needed for prime_index().   */
/* ------------------------------------------------------------------ */
static uint32_t *prime_list = NULL;
static size_t   prime_cnt   = 0;
static size_t   prime_cap   = 0;
static uint64_t prime_limit = 0;

/* enlarge the prime list to contain all primes ≤ limit (inclusive) */
static void ensure_primes(uint64_t limit)
{
    if (limit <= prime_limit) return;
    if (prime_limit < 2) prime_limit = 1;

    for (uint64_t cand = prime_limit + 1; cand <= limit; ++cand) {
        if (cand < 2) continue;
        bool is_prime = true;
        uint64_t sq = u64_sqrt(cand);
        for (size_t i = 0; i < prime_cnt && prime_list[i] <= sq; ++i) {
            if (cand % prime_list[i] == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            if (prime_cnt == prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 16;
                prime_list = (uint32_t*)realloc(prime_list,
                                                prime_cap * sizeof(uint32_t));
            }
            prime_list[prime_cnt++] = (uint32_t)cand;
        }
    }
    prime_limit = limit;
}

/* binary search – return the 1‑based index of prime p (p must be prime). */
static inline uint64_t prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] < p) lo = mid + 1;
        else                     hi = mid;
    }
    return lo + 1;   /* 1‑based */
}

/* ------------------------------------------------------------------ */
/* OEIS sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* a(1) = 0 (empty factorisation) */
    if (n <= 1) return (seq_t)0;

    /* 1. factorise n and build the list of prime indices (with multiplicity) */
    uint64_t v     = (uint64_t)n;
    uint64_t *idx  = NULL;
    size_t    idx_len = 0, idx_cap = 0;

    /* trial division using primes up to sqrt(v) */
    ensure_primes(u64_sqrt(v));

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_list[i];
        if ((uint64_t)p * p > v) break;
        if (v % p) continue;

        uint64_t g = 0;
        while (v % p == 0) { v /= p; ++g; }

        uint64_t ip = prime_index(p);

        /* ensure capacity for g copies */
        if (idx_len + g > idx_cap) {
            idx_cap = idx_cap ? idx_cap * 2 : 16;
            idx = (uint64_t*)realloc(idx, idx_cap * sizeof(uint64_t));
        }
        for (uint64_t k = 0; k < g; ++k) idx[idx_len++] = ip;
    }

    /* remaining prime factor > 1 */
    if (v > 1) {
        ensure_primes(v);
        uint64_t ip = prime_index(v);
        if (idx_len + 1 > idx_cap) {
            idx_cap = idx_cap ? idx_cap * 2 : 16;
            idx = (uint64_t*)realloc(idx, idx_cap * sizeof(uint64_t));
        }
        idx[idx_len++] = ip;
    }

    /* 2. run‑length transform until singleton */
    uint64_t steps = 0;
    while (idx_len > 1) {
        uint64_t *next = (uint64_t*)malloc(idx_len * sizeof(uint64_t));
        size_t next_len = 0;

        for (uint64_t i = 0; i < idx_len; ) {
            uint64_t val = idx[i];
            uint64_t j = i + 1;
            while (j < idx_len && idx[j] == val) ++j;
            next[next_len++] = j - i;
            i = j;
        }

        free(idx);
        idx = next;
        idx_len = next_len;
        ++steps;
    }
    free(idx);

    return (seq_t)steps;
}
