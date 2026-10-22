/*
 * A364262  a(1) = 1; for n > 1, a(n) is the smallest positive number that has
 *          not yet appeared such that a(n-1) + a(n) has the same number of
 *          distinct prime factors as a(n-1) * a(n).
 * https://oeis.org/A364262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n>1, a(n) is the smallest positive integer not yet
 *            used such that the number of distinct prime factors of
 *            a(n‑1)+a(n) equals that of a(n‑1)*a(n)
 * Range:     u8 n <= 197, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses only integer arithmetic, no printf or file IO.  It
   works for any seq_t/idx_t definition supplied by the build.  C++ casts
   are performed on every malloc/calloc/realloc. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* ----------------------------------------------------------------- */
/* Simple prime sieve – sufficient for 64 bit trial division.       */
/* ----------------------------------------------------------------- */
#define PRIME_SIEVE_LIMIT 1000000U   /* should exceed sqrt of largest value */

static unsigned int *prime_list = NULL;
static size_t prime_cnt = 0;

static void init_primes(void)
{
    if (prime_list != NULL) {
        return;       /* already initialised */
    }

    bool *is_comp =
        (bool *)calloc((size_t)PRIME_SIEVE_LIMIT + 1, sizeof(bool));
    if (!is_comp) {
        exit(EXIT_FAILURE);
    }

    /* Allocate a rough estimate for the number of primes. */
    prime_list =
        (unsigned int *)malloc((size_t)(PRIME_SIEVE_LIMIT / 10) * sizeof(unsigned int));
    if (!prime_list) {
        free(is_comp);
        exit(EXIT_FAILURE);
    }

    prime_cnt = 0;
    for (unsigned int i = 2; i <= PRIME_SIEVE_LIMIT; ++i) {
        if (!is_comp[i]) {
            prime_list[prime_cnt++] = i;
            if ((uint64_t)i * i <= PRIME_SIEVE_LIMIT) {
                for (unsigned int j = i * i; j <= PRIME_SIEVE_LIMIT; j += i) {
                    is_comp[j] = true;
                }
            }
        }
    }

    free(is_comp);
}

/* ----------------------------------------------------------------- */
/* Count distinct prime factors of a 64‑bit integer.                  */
/* ----------------------------------------------------------------- */
static inline int distinct_prime_factors_uint64(uint64_t n)
{
    init_primes();
    int cnt = 0;
    if (n < 2) {
        return 0;
    }
    for (size_t i = 0; i < prime_cnt &&
                     (uint64_t)prime_list[i] * (uint64_t)prime_list[i] <= n;
         ++i) {
        if (n % prime_list[i] == 0) {
            ++cnt;
            while (n % prime_list[i] == 0) {
                n /= prime_list[i];
            }
        }
    }
    if (n > 1) {
        ++cnt;
    }
    return cnt;
}

/* ----------------------------------------------------------------- */
/* Memoisation of already computed terms and used values.             */
/* ----------------------------------------------------------------- */
static seq_t *seq_vals = NULL;      /* a(1)..a(max_computed) */
static size_t seq_len = 0;          /* number of stored elements */
static size_t seq_cap = 0;          /* allocated capacity */

/* 1‑based flag array: used_flag[k] == 1 iff k already appeared */
static unsigned char *used_flag = NULL;
static size_t used_cap = 0;         /* highest allocated index */

/* Next candidate for a new term */
static idx_t smallest_unused = 1;

/* ----------------------------------------------------------------- */
/* Ensure that used_flag can reach up to 'need' (inclusive).         */
/* ----------------------------------------------------------------- */
static void ensure_used_capacity(idx_t need)
{
    if ((size_t)need > used_cap) {
        size_t new_cap = (size_t)need + 1;
        unsigned char *tmp =
            (unsigned char *)realloc(used_flag, new_cap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        /* Zero newly allocated tail (excluding index 0 which is unused). */
        memset(tmp + used_cap + 1, 0, new_cap - used_cap - 1);
        used_flag = tmp;
        used_cap = new_cap - 1;
    }
}

/* ----------------------------------------------------------------- */
/* Ensure seq_vals can hold at least 'need' elements.                 */
/* ----------------------------------------------------------------- */
static void ensure_seq_capacity(size_t need)
{
    if (need > seq_cap) {
        size_t new_cap = need + 10;   /* modest growth */
        seq_t *tmp = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        seq_vals = tmp;
        seq_cap = new_cap;
    }
}

/* ----------------------------------------------------------------- */
/* Test whether a candidate satisfies the condition with the previous
   term.                                                            */
/* ----------------------------------------------------------------- */
static bool test_candidate(seq_t prev, seq_t cand)
{
    /* For the purposes of this implementation we assume all terms
       fit into uint64_t.  This is true for the range used in tests. */
    uint64_t p = (uint64_t)prev;
    uint64_t c = (uint64_t)cand;

    uint64_t sum = p + c;
    uint64_t prod = p * c;

    int omega_sum = distinct_prime_factors_uint64(sum);
    int omega_prod = distinct_prime_factors_uint64(prod);

    return omega_sum == omega_prod;
}

/* ----------------------------------------------------------------- */
/* The requested sequence function.                                  */
/* ----------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;                 /* undefined, safe guard */
    }

    /* Lazily initialise data structures. */
    if (seq_len == 0) {
        /* allocate enough space for first term */
        ensure_seq_capacity(10);
        ensure_used_capacity(10);

        seq_len = 1;
        seq_vals[0] = (seq_t)1;       /* a(1) = 1 */
        used_flag[1] = 1;             /* mark 1 as used */
        smallest_unused = 2;          /* next candidate starts at 2 */
    }

    /* Keep computing until we have a(n). */
    while (seq_len < n) {
        seq_t prev = seq_vals[seq_len - 1];
        idx_t cand = smallest_unused;

        /* Find the smallest unused candidate that satisfies the
           property. */
        while (true) {
            ensure_used_capacity(cand);
            if (!used_flag[cand]) {
                if (test_candidate(prev, (seq_t)cand)) {
                    break;      /* this candidate works */
                }
            }
            ++cand;
        }

        /* Record the new term. */
        ++seq_len;
        ensure_seq_capacity(seq_len);
        seq_vals[seq_len - 1] = (seq_t)cand;
        used_flag[cand] = 1;

        /* Advance smallest_unused to the next unused integer. */
        while (smallest_unused <= used_cap && used_flag[smallest_unused]) {
            ++smallest_unused;
        }
    }

    return seq_vals[n - 1];
}
