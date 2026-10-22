/*
 * A331384  Numbers whose sum of prime factors is equal to their product of
 *          prime indices.
 * https://oeis.org/A331384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th integer k (n ≥ 1) such that
 *            sum_of_prime_factors(k) = product_of_prime_indices(k)
 * Range:     u8 n <= 5, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External linkage as required */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static state for the sieve and cache */
static idx_t *spf = NULL;          /* smallest prime factor for each number */
static uint64_t *prime_list = NULL;/* list of primes up to current limit */
static uint64_t *prime_index_of = NULL; /* 1‑based index of each prime */
static idx_t sieve_limit = 0;       /* maximum number sieved */

static seq_t *cached_terms = NULL; /* array of found sequence values */
static size_t cached_size = 0;     /* number of terms stored */
static size_t cached_cap = 0;      /* allocated capacity */

/* ------------------------------------------------------------------------ */
/* Utility: power of a seq_t base to a non‑negative integer exponent */
static seq_t pow_seq_exp(seq_t base, unsigned exp)
{
    seq_t res = 1;
    while (exp-- > 0)
        res *= base;
    return res;
}

/* ------------------------------------------------------------------------ */
/* Ensure that all arrays are built up to at least 'need' (inclusive).      */
static void ensure_limit(idx_t need)
{
    if (sieve_limit >= need)
        return;

    /* Reallocate arrays to new size */
    size_t sz = (size_t)need + 1;

    /* free previous data */
    if (spf) free(spf);
    if (prime_list) free(prime_list);
    if (prime_index_of) free(prime_index_of);

    spf = (idx_t*)calloc(sz, sizeof(idx_t));
    prime_list = (uint64_t*)calloc(sz, sizeof(uint64_t));
    prime_index_of = (uint64_t*)calloc(sz, sizeof(uint64_t));

    if (!spf || !prime_list || !prime_index_of) {
        /* allocation failed – abort; the build environment should provide
           enough memory for the required limits */
        exit(EXIT_FAILURE);
    }

    /* Linear sieve to compute spf and primes */
    size_t pn = 0; /* number of primes found */
    for (idx_t i = 2; i <= need; ++i) {
        if (spf[i] == 0) {          /* i is prime */
            spf[i] = i;
            prime_list[pn++] = (uint64_t)i;
        }
        for (size_t j = 0; j < pn; ++j) {
            idx_t p = prime_list[j];
            idx_t prod = i * p;
            if (prod > need)
                break;
            spf[prod] = p;
            if (p == spf[i])
                break;
        }
    }

    /* Build mapping from prime to its index (1‑based) */
    for (size_t k = 0; k < pn; ++k)
        prime_index_of[prime_list[k]] = k + 1;

    sieve_limit = need;
}

/* ------------------------------------------------------------------------ */
/* Test whether number 'num' satisfies the defining condition.             */
static bool matches(idx_t num)
{
    if (num < 2)
        return false;

    seq_t sum_pf = 0;
    seq_t prod_idx = 1;
    idx_t x = num;

    while (x > 1) {
        idx_t p = spf[x];
        if (p == 0)          /* safety for prime numbers not covered by sieve */
            p = x;
        unsigned exp = 0;
        while (x % p == 0) {
            x /= p;
            ++exp;
        }
        sum_pf += (seq_t)p * exp;
        seq_t idx_val = (seq_t)prime_index_of[p];
        prod_idx *= pow_seq_exp(idx_val, exp);
    }
    return sum_pf == prod_idx;
}

/* ------------------------------------------------------------------------ */
/* Grow the cached terms array to at least 'need' entries.                  */
static void grow_cache(size_t need)
{
    if (cached_cap >= need)
        return;
    size_t new_cap = cached_cap ? cached_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    cached_terms = (seq_t*)realloc(cached_terms, new_cap * sizeof(seq_t));
    if (!cached_terms)
        exit(EXIT_FAILURE);
    cached_cap = new_cap;
}

/* ------------------------------------------------------------------------ */
/* Main sequence function                                                        */
seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1; n <= 0 returns 0 */
        return 0;

    /* Initial sieve limit – can be enlarged dynamically. */
    static bool init = false;
    if (!init) {
        init = true;
        /* A rough upper bound for 10000th term is unknown; start with 1e6. */
        ensure_limit((idx_t)1000000);
    }

    /* Keep a global next candidate and generate terms as needed. */
    static idx_t next_candidate = 2;
    static bool cand_initialized = false;
    if (!cand_initialized) {
        cand_initialized = true;
    }

    /* Generate terms until we have at least n stored. */
    while (cached_size < (size_t)n) {
        /* Ensure the sieve covers the next candidate. */
        if (next_candidate > sieve_limit) {
            idx_t new_limit = sieve_limit * 2;
            if (new_limit < next_candidate * 2)
                new_limit = next_candidate * 2;
            ensure_limit(new_limit);
        }

        if (matches(next_candidate)) {
            grow_cache(cached_size + 1);
            cached_terms[cached_size++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    return cached_terms[n - 1];
}
