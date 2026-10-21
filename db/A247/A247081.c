/*
 * A247081  Positive integers k such that the numerator of the harmonic mean
 *          of the nontrivial divisors of k is equal to k.
 * https://oeis.org/A247081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 711, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A247081: integer k such that the numerator of the harmonic
 * mean of its non‑trivial divisors equals k.
 *
 * For k, let τ(k) be the number of divisors and σ(k) the sum of
 * divisors.  non‑trivial divisors are all divisors excluding 1 and k.
 * With m = τ(k)-2 and T = σ(k)-k-1 we have
 *
 *     HarmonicMean = (m * k) / T .
 *
 * After reducing this fraction to lowest terms the numerator equals k
 * iff   gcd(m*k, T) = m   <=>   m | T  and  gcd(k, T/m) = 1.
 *
 * The function below generates the sequence by testing successive
 * integers until the required number of terms is found.  The sequence
 * is cached so that any order of calls works in < 2 s for n up to ~10 000.
 */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"

/* ---------- cache for the sequence ---------- */
static seq_t *cache = NULL;     /* array of terms */
static size_t cache_len = 0;    /* number of stored terms */
static size_t cache_cap = 0;    /* allocated capacity */
static uint64_t next_candidate = 2;   /* next integer to test */

/* enlarge cache when needed */
static void grow_cache(size_t new_cap)
{
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) abort();   /* out of memory */
    cache = new_ptr;
    cache_cap = new_cap;
}

/* append a term to the cache */
static void add_term(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        grow_cache(new_cap);
    }
    cache[cache_len++] = val;
}

/* test whether a given k satisfies the condition */
static bool candidate_ok(uint64_t k)
{
    /* factorisation */
    uint64_t primes[64], exps[64];
    size_t pf_cnt = 0;
    factor_u64(k, primes, exps, &pf_cnt);

    /* count divisors τ(k) and sum of divisors σ(k) */
    seq_t tau = 1;
    seq_t sigma = 1;
    for (size_t i = 0; i < pf_cnt; ++i) {
        uint64_t p = primes[i];
        uint32_t e = (uint32_t)exps[i];

        /* τ contribution */
        tau *= (seq_t)(e + 1);

        /* σ contribution = (p^(e+1)-1)/(p-1) */
        seq_t p_pow = 1;
        for (uint32_t j = 0; j < e + 1; ++j)
            p_pow *= (seq_t)p;              /* p^(e+1) */

        seq_t term = (p_pow - 1) / (seq_t)(p - 1);
        sigma *= term;
    }

    /* no non‑trivial divisors */
    if (tau <= 2) return false;

    seq_t m = tau - 2;                     /* τ(k)-2   */
    seq_t k_seq = (seq_t)k;
    seq_t T = sigma - k_seq - (seq_t)1;    /* σ(k)-k-1 */

    /* check m | T  ->  T % m == 0 */
    seq_t q = T / m;                       /* integer division */
    if (q * m != T) return false;

    /* gcd(k, q) == 1 */
    if (gcd(k_seq, q) != (seq_t)1) return false;

    return true;
}

/* ---------- public sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, safe return */

    /* generate terms until we have at least n */
    while (cache_len < (size_t)n) {
        if (candidate_ok(next_candidate))
            add_term((seq_t)next_candidate);
        ++next_candidate;
    }
    return cache[(size_t)n - 1];
}
