/*
 * A246371  Numbers n such that, if 2n-1 = Product_{k >= 1} (p_k)^(c_k) then n
 *          > Product_{k >= 1} (p_{k-1})^(c_k), where p_k indicates the k-th
 *          prime, A000040(k).
 * https://oeis.org/A246371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A246371 is defined by the property that for a number n let 2n-1
 *            = ∏ pk^ck (p_k is the k‑th prime). Then n is in the sequence iff
 *            n > ∏ p_{k-1}^ck. The function a(n) returns the nth term (offset
 *            1)
 * Range:     u8 n <= 37, u16 n <= 9999, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, factor_u64, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *seq_cache = NULL;      /* cached terms   */
static size_t seq_size  = 0;         /* number of terms in cache */
static size_t seq_cap   = 0;         /* allocated capacity */

static uint64_t *prime_list = NULL;  /* list of primes up to prime_limit */
static size_t prime_cnt   = 0;       /* number of primes in prime_list */
static const size_t prime_limit = 200000;   /* sufficiently large for n≤10000 */

/* Helper to find the index of a prime in prime_list using binary search. */
static inline size_t prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

/* Check whether a candidate m satisfies the property. */
static bool passes(seq_t m)
{
    /* 2m-1 fits into 64 bit for the intended usage range */
    uint64_t x = 2ULL * (uint64_t)m - 1;
    /* buffers for factorisation –
       more than enough for the numbers we encounter */
    uint64_t pf[64], ex[64];
    size_t cnt;
    factor_u64(x, pf, ex, &cnt);

    seq_t prod = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pf[i];
        /* preceding prime of p */
        size_t idx = prime_index(p);
        if (idx == 0) /* should never happen (p=2 never divides 2m-1) */
            return false;
        uint64_t prev = prime_list[idx - 1];
        seq_t term = 1;
        for (int e = 0; e < (int)ex[i]; ++e)
            term *= (seq_t)prev;
        prod *= term;
        /* early exit if we already reach or exceed m */
        if (prod >= m)
            return false;
    }
    return m > prod;
}

/* Ensure the cache contains at least n terms. */
static void ensure_terms(size_t n)
{
    if (seq_cache == NULL) {
        seq_cache = seq_alloc(16);
        seq_cap = 16;
    }
    /* Build a list of primes once, if not yet done. */
    if (prime_list == NULL) {
        build_primes_upto(prime_limit, &prime_list, &prime_cnt);
    }

    while (seq_size < n) {
        seq_t candidate = (seq_size == 0) ? 1 : seq_cache[seq_size - 1] + 1;
        /* Find the next term that satisfies the property. */
        while (!passes(candidate))
            ++candidate;

        /* grow cache if necessary */
        if (seq_size == seq_cap) {
            seq_cap <<= 1; /* double capacity */
            seq_cache = seq_realloc(seq_cache, seq_size, seq_cap);
        }
        seq_cache[seq_size++] = candidate;
    }
}

/* External linkage as required. */
seq_t a(idx_t n)
{
    /* n≥1 assumed by the caller; behaviour for n<1 is undefined. */
    ensure_terms((size_t)n);
    return seq_cache[n - 1];
}
