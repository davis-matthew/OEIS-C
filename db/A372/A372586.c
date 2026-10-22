/*
 * A372586  Numbers k such that (sum of binary indices of k) + (sum of prime
 *          indices of k) is odd.
 * https://oeis.org/A372586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth positive integer k such that (sum of binary
 *            indices of k) + (sum of prime indices of k) is odd
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/build_spf.h"
#include "../utils/build_primes_upto.h"

/* static state for memoisation */
static seq_t *terms = NULL;            /* array of found terms */
static size_t terms_cap = 0;           /* capacity of terms array */
static size_t term_cnt  = 0;           /* number of terms stored */
static idx_t last_k     = 0;           /* last checked integer */

/* small‑prime‑factor table */
static idx_t spf_limit = 0;            /* current maximal range of spf table */
static idx_t *spf = NULL;              /* spf[n] = smallest prime factor of n */

/* prime‑index table: prime_index[p] = 1‑based index of prime p (0 if not prime) */
static idx_t *prime_index = NULL;

/* Ensure that spf and prime_index cover all integers <= needed_limit. */
static void
ensure_tables(idx_t needed_limit)
{
    if (needed_limit <= spf_limit)
        return;

    /* grow to next power of two larger than needed_limit */
    idx_t new_limit = spf_limit ? spf_limit : 128;
    while (new_limit < needed_limit)
        new_limit <<= 1;

    /* build SPF table up to new_limit */
    idx_t *new_spf = NULL;
    build_spf(new_limit, &new_spf);
    if (spf) free(spf);
    spf = new_spf;

    /* build list of primes up to new_limit */
    uint64_t *prime_arr = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(new_limit, &prime_arr, &prime_cnt);

    /* free old prime_index and create new one */
    if (prime_index) free(prime_index);
    prime_index = (idx_t *)calloc((size_t)new_limit + 1, sizeof(idx_t));
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = (idx_t)prime_arr[i];
        prime_index[p] = (idx_t)(i + 1);   /* 1‑based index for prime p */
    }

    free(prime_arr);
    spf_limit = new_limit;
}

/* Return true iff k satisfies the parity condition. */
static bool
odd_sum_condition(idx_t k)
{
    /* Sum of binary indices (positions of 1‑bits, 1‑based). */
    unsigned long long b_sum = 0;
    idx_t x = k;
    idx_t pos = 1;
    while (x) {
        if (x & 1) b_sum += pos;
        x >>= 1;
        pos++;
    }

    /* Sum of prime indices (multiplicity). */
    unsigned long long p_sum = 0;
    idx_t y = k;
    while (y > 1) {
        idx_t p = spf[y];
        p_sum += prime_index[p];
        y /= p;
    }

    return ((b_sum + p_sum) & 1ULL) != 0;
}

/* External function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* offset is 1; unused value for n==0 */

    /* Initialise term storage on first call. */
    if (!terms) {
        terms_cap = 256;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        term_cnt  = 0;
        last_k    = 0;
    }

    /* Generate terms until we have n of them. */
    while (term_cnt < n) {
        idx_t candidate = last_k + 1;

        if (candidate > spf_limit)
            ensure_tables(candidate);

        if (odd_sum_condition(candidate)) {
            if (term_cnt == terms_cap) {
                terms_cap <<= 1;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[term_cnt++] = (seq_t)candidate;
        }

        last_k = candidate;
    }

    return terms[n - 1];   /* terms array is 0‑based */
}
