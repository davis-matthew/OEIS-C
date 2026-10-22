/*
 * A364461  Positive integers such that if prime(a)*prime(b) is a divisor,
 *          prime(a+b) is not.
 * https://oeis.org/A364461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 223, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, factor_u64, free_seq, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) = the n-th positive integer whose multiset of prime indices
   (indices of the prime factors counted with multiplicity) is sum‑free:
   for any two occurrences a and b that occur in the factorization,
   the index a+b cannot occur among the indices.
*/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"
#include "../utils/seq_realloc.h"
#include "../utils/free_seq.h"

/* Build a prime table once. */
static uint64_t *prime_tbl = NULL;
static size_t prime_cnt  = 0;

static void
ensure_primes(void)
{
    if (!prime_tbl)
    {
        /* 2 000 000 > enough for all terms up to about 10 000. */
        const uint64_t LIM = 2000000ULL;
        build_primes_upto(LIM, &prime_tbl, &prime_cnt);
    }
}

/* 1‑based index of a prime in the table (prime assumed to be in the set). */
static uint64_t
prime_index(uint64_t p)
{
    ensure_primes();
    size_t lo = 0, hi = prime_cnt - 1;
    while (lo <= hi)
    {
        size_t mid = (lo + hi) >> 1;
        uint64_t v   = prime_tbl[mid];
        if (v == p) return (uint64_t)(mid + 1);          /* 1‑based */
        if (v <  p)  lo  = mid + 1;
        else         hi  = mid - 1;
    }
    return 0;   /* should never happen for a prime argument */
}

/*  Test whether n satisfies the sum‑free index property. */
static bool
is_good(uint64_t n)
{
    if (n == 1) return true;   /* 1 has an empty factorisation */

    uint64_t pf[64], ex[64];
    size_t pc = 0;
    factor_u64(n, pf, ex, &pc);

    /* Expand to a list of indices with multiplicity. */
    uint64_t idx[160];
    size_t tot = 0;
    for (size_t i = 0; i < pc; ++i)
    {
        uint64_t idx_val = prime_index(pf[i]);   /* 1‑based */
        for (uint64_t k = 0; k < ex[i]; ++k)
            idx[tot++] = idx_val;
    }

    /* Check sum‑free condition: no two distinct occurrences add
       to a third occurring index. */
    for (size_t i = 0; i < tot; ++i)
    {
        for (size_t j = i + 1; j < tot; ++j)    /* i < j guarantees distinct positions */
        {
            uint64_t s = idx[i] + idx[j];
            for (size_t k = 0; k < tot; ++k)
                if (idx[k] == s) return false;
        }
    }
    return true;
}

/* --------- Cache of computed terms --------- */
static seq_t *cache   = NULL;   /* array of terms */
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static uint64_t next_to_test = 1;   /* next integer to examine */

/* Append a new term to the cache. */
static void
cache_push(seq_t val)
{
    if (cache_len == cache_cap)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    while (cache_len < (size_t)n)
    {
        if (is_good(next_to_test))
            cache_push((seq_t)next_to_test);
        ++next_to_test;
    }
    return cache[(size_t)n - 1];
}
