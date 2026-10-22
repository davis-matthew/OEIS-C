/*
 * A381632  Numbers such that (greatest prime exponent) = (sum of distinct
 *          prime indices).
 * https://oeis.org/A381632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=2; a(n) is the n-th smallest positive integer whose prime
 *            factorization satisfies (greatest exponent of any prime factor)
 *            = (sum of distinct prime indices). The prime index of a prime p
 *            is its position in the list of primes (index(2)=1, index(3)=2,
 *            …). This implementation builds the sequence...
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef __SEQ_A381632_H
#define __SEQ_A381632_H

#include <stdint.h>
#include <stddef.h>
#ifdef __cplusplus
#include <vector>
#endif

/* external integer types defined by the build */
extern seq_t a(idx_t n);

#ifdef __cplusplus
/* --- C++ implementation ------------------------------------------------- */
static std::vector<seq_t>              terms;          /* computed sequence   */
static std::vector<uint32_t>           spf;            /* smallest prime factor */
static std::vector<uint32_t>           prime_index;    /* index of each prime   */
static idx_t                          spf_limit = 0; /* largest n sieved     */
static idx_t                          next_candidate = 2; /* next integer to test */

static void ensure_spf_limit(idx_t limit)
{
    if (limit <= spf_limit) return;

    spf.resize(limit + 1);
    prime_index.resize(limit + 1);
    std::vector<uint32_t> primes;
    primes.reserve(limit / 10 + 10);

    for (idx_t i = 0; i <= limit; ++i) {
        spf[i] = (uint32_t)i;
        prime_index[i] = 0;
    }

    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == (uint32_t)i) {                /* i is prime */
            primes.push_back((uint32_t)i);
            prime_index[i] = (uint32_t)primes.size();
        }
        for (uint32_t p : primes) {
            uint64_t prod = (uint64_t)i * p;
            if (prod > limit) break;
            spf[(idx_t)prod] = p;
            if (i % p == 0) break;
        }
    }
    spf_limit = limit;
}

static void generate_terms(size_t required)
{
    while (terms.size() < required) {
        ensure_spf_limit(next_candidate);
        idx_t  m   = next_candidate;
        uint32_t max_exp = 0;
        uint32_t sum_idx = 0;
        idx_t  tmp = m;
        while (tmp > 1) {
            uint32_t p = spf[(idx_t)tmp];
            uint32_t exp = 0;
            while (tmp % p == 0) { tmp /= p; ++exp; }
            if (exp > max_exp) max_exp = exp;
            sum_idx += prime_index[p];
        }
        if (max_exp == sum_idx)
            terms.push_back((seq_t)m);
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset 1, undefined for 0 */
    generate_terms((size_t)n);
    return terms[(size_t)n - 1];
}
#else
/* --- C implementation ---------------------------------------------------- */
#include <stdlib.h>

static seq_t  *terms       = NULL;  /* dynamic array of sequence values */
static size_t  terms_count = 0;      /* number of terms stored   */
static size_t  terms_cap   = 0;      /* capacity of the array   */

static uint32_t *spf         = NULL;   /* smallest prime factor array */
static uint32_t *prime_index = NULL;   /* index of each prime     */
static size_t  spf_limit    = 0;       /* largest n sieved       */

static idx_t   next_candidate = 2;     /* next integer to test   */

/* Ensure spf and prime_index are computed up to at least 'limit'          */
static void ensure_spf_limit(idx_t limit)
{
    if (limit <= spf_limit) return;

    /* grow spf and prime_index arrays */
    spf = (uint32_t*)realloc(spf, (size_t)(limit + 1) * sizeof(uint32_t));
    prime_index = (uint32_t*)realloc(prime_index,
                                 (size_t)(limit + 1) * sizeof(uint32_t));
    if (!spf || !prime_index) { /* allocation failure */
        exit(1);
    }

    for (idx_t i = 0; i <= limit; ++i) {
        spf[i] = (uint32_t)i;
        prime_index[i] = 0;
    }

    /* linear sieve to fill spf and primes list */
    size_t primes_cap = limit / 10 + 10;
    uint32_t *primes = (uint32_t*)malloc(primes_cap * sizeof(uint32_t));
    if (!primes) exit(1);
    size_t primes_cnt = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == (uint32_t)i) {            /* i is prime */
            if (primes_cnt == primes_cap) {     /* grow primes buffer */
                primes_cap *= 2;
                primes = (uint32_t*)realloc(primes,
                                  primes_cap * sizeof(uint32_t));
                if (!primes) exit(1);
            }
            primes[primes_cnt++] = (uint32_t)i;
            prime_index[i] = (uint32_t)primes_cnt;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            uint64_t prod = (uint64_t)i * primes[j];
            if (prod > limit) break;
            spf[(idx_t)prod] = primes[j];
            if (i % primes[j] == 0) break;
        }
    }
    free(primes);
    spf_limit = limit;
}

/* Make sure 'terms' array can hold at least 'needed' elements          */
static void ensure_terms_capacity(size_t needed)
{
    if (terms_cap >= needed) return;
    size_t new_cap = terms_cap == 0 ? 16 : terms_cap * 2;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_terms = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
    if (!new_terms) exit(1);
    terms     = new_terms;
    terms_cap = new_cap;
}

/* Generate sequence terms until we have at least 'required' terms        */
static void generate_terms(size_t required)
{
    while (terms_count < required) {
        ensure_spf_limit(next_candidate);
        idx_t  m   = next_candidate;
        uint32_t max_exp = 0;
        uint32_t sum_idx = 0;
        idx_t  tmp = m;
        while (tmp > 1) {
            uint32_t p = spf[(idx_t)tmp];
            uint32_t exp = 0;
            while (tmp % p == 0) { tmp /= p; ++exp; }
            if (exp > max_exp) max_exp = exp;
            sum_idx += prime_index[p];
        }
        if (max_exp == sum_idx) {
            ensure_terms_capacity(terms_count + 1);
            terms[terms_count++] = (seq_t)m;
        }
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0; /* sequence is offset 1 */
    generate_terms((size_t)n);
    return terms[(size_t)n - 1];
}
#endif /* __cplusplus */
#endif /* __SEQ_A381632_H */
