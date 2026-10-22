/*
 * A358731  Matula-Goebel numbers of rooted trees whose number of nodes is one
 *          more than their node-height.
 * https://oeis.org/A358731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) is the n-th positive integer whose Matula‑Goebel tree has
 *            node‑count = node‑height + 1
 * Range:     u8 n <= 15, u16 n <= 34, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* limits for the sieve */
#define SIEVE_LIMIT 20000000
#define MAX_PRIMES  1300000

/* global arrays for smallest prime factor and prime list */
static uint32_t spf[SIEVE_LIMIT + 1];
static uint32_t primes[MAX_PRIMES];
static size_t   prime_cnt = 0;

/* initialise the sieve and prime list */
static void init_sieve(void)
{
    spf[0] = 0;
    spf[1] = 1;
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (prime_cnt < MAX_PRIMES) {
                primes[prime_cnt++] = (uint32_t)i;
            }
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint32_t p = primes[j];
            uint64_t prod = (uint64_t)i * p;
            if (prod > (uint64_t)SIEVE_LIMIT) break;
            spf[(size_t)prod] = p;
            if (p == spf[i]) break;
        }
    }
}

/* binary search: index (1‑based) of prime p in the primes array */
static idx_t prime_index(uint32_t p)
{
    size_t l = 0, r = prime_cnt;
    while (l < r) {
        size_t m = (l + r) >> 1;
        if (primes[m] < p) l = m + 1;
        else r = m;
    }
    return (idx_t)(l + 1);          /* primes are 1‑indexed in the MG construction */
}

/* recursive computation of the number of nodes and the node‑height of a tree */
static void computeSizeHeight(idx_t n, uint32_t *sizep, uint32_t *heightp)
{
    if (n == (idx_t)1) {
        *sizep   = 1;
        *heightp = 1;
        return;
    }

    uint32_t tmp = (uint32_t)n;
    uint32_t total_size = 1;
    uint32_t max_height = 0;

    while (tmp > 1) {
        uint32_t p   = spf[tmp];
        uint32_t cnt = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++cnt;
        }
        idx_t child_idx = prime_index(p);

        uint32_t child_size, child_height;
        computeSizeHeight(child_idx, &child_size, &child_height);

        /* the same child appears 'cnt' times */
        for (uint32_t i = 0; i < cnt; ++i) {
            total_size += child_size;
            if (child_height > max_height) max_height = child_height;
        }
    }

    *sizep   = total_size;
    *heightp = 1 + max_height;
}

/* cache of the sequence terms */
static seq_t *seq_cache  = NULL;
static size_t seq_cache_len = 0;
static idx_t  next_candidate = 2;
static bool   sieve_inited   = false;

/* ensure the cache contains at least 'target' terms */
static void ensure_terms(idx_t target)
{
    if (!sieve_inited) {
        init_sieve();
        sieve_inited = true;
    }

    while (seq_cache_len < target) {
        uint32_t sz, ht;
        computeSizeHeight(next_candidate, &sz, &ht);
        if (sz == ht + 1) {                     /* valid term */
            seq_cache = (seq_t *)realloc(seq_cache, (seq_cache_len + 1) * sizeof(seq_t));
            seq_cache[seq_cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
}

/* externally visible function */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;          /* undefined for n < 1, return 0 as safe default */
    ensure_terms(n);
    return seq_cache[n - 1];
}
