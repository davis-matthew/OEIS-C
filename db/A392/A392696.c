/*
 * A392696  Numbers whose reversed prime indices are the 0-prepended first
 *          sums of some finite sequence of nonnegative integers.
 * https://oeis.org/A392696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 7184, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A392696: numbers whose reversed prime indices are the 0‑prepended
 * first sums of some finite sequence of nonnegative integers.
 * Computed by sieving integers and testing the alternating‑sum property.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <inttypes.h>
#include <limits.h>
#include "../utils/build_spf.h"
#include "../utils/build_primes_upto.h"

#define SIEVE_LIMIT 2000000ULL   /* sufficient for the required range */

static idx_t *spf = NULL;          /* smallest prime factor */
static idx_t *prime_index = NULL;  /* 1‑based index of each prime */
static idx_t spf_cap = 0;

/* cache of qualifying numbers */
static seq_t *qual = NULL;
static size_t qual_cap = 0;
static idx_t qual_size = 0;
static idx_t next_candidate = 1;

/* initialise sieve data and prime indices */
static void init_sieve(void)
{
    if (spf != NULL) return; /* already initialised */

    /* build SPF table */
    build_spf(SIEVE_LIMIT, &spf);
    spf_cap = SIEVE_LIMIT;

    /* build list of primes */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(SIEVE_LIMIT, &primes, &prime_cnt);

    /* allocate prime_index array */
    prime_index = (idx_t *)malloc((SIEVE_LIMIT + 1) * sizeof(idx_t));
    if (!prime_index) abort();
    for (idx_t i = 0; i <= SIEVE_LIMIT; ++i) prime_index[i] = 0;

    /* assign 1‑based indices to primes */
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = (idx_t)primes[i];
        prime_index[p] = (idx_t)(i + 1);
    }

    free(primes);
}

/* append a new qualifying number to the cache */
static void append_qual(seq_t val)
{
    if (qual_size == qual_cap) {
        size_t new_cap = (qual_cap == 0) ? 4 : qual_cap * 2;
        seq_t *tmp = (seq_t *)realloc(qual, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        qual = tmp;
        qual_cap = new_cap;
    }
    qual[qual_size++] = val;
}

/* test whether x satisfies the property */
static bool is_qualifying(idx_t x)
{
    if (x == 1) return true;   /* 1 has no prime indices */

    /* collect prime indices in ascending order (ascending p -> ascending index) */
    idx_t indices[128];
    size_t cnt = 0;
    idx_t tmp = x;

    while (tmp > 1 && cnt < sizeof(indices)/sizeof(indices[0])) {
        idx_t p = spf[tmp];
        idx_t idx = prime_index[p];
        /* repeat for multiplicity */
        while (tmp % p == 0) {
            if (cnt == sizeof(indices)/sizeof(indices[0])) return false; /* overflow guard */
            indices[cnt++] = idx;
            tmp /= p;
        }
    }
    if (tmp > 1) return false; /* factors beyond SIEVE_LIMIT – unlikely for required range */

    /* compute 0‑based partial alternating sums of reversed indices */
    /* recurrence: sum_j = -sum_{j-1} + q_j where q_j is j‑th element of reversed list */
    int64_t sum = 0;
    for (size_t j = 0; j < cnt; ++j) {
        /* q_j = indices in reverse order */
        idx_t q = indices[cnt - 1 - j];
        sum = -sum + (int64_t)q;
        if (sum < 0) return false;
    }
    return true;
}

/* public function: returns a(n) of sequence A392696 */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        init_sieve();
        initialized = true;
    }

    while (qual_size < n) {
        if (is_qualifying(next_candidate))
            append_qual((seq_t)next_candidate);
        ++next_candidate;
    }

    return qual[n - 1];
}
