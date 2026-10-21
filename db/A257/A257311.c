/*
 * A257311  a(1) = 4; a(2) = 5; for n > 2, a(n) is the smallest number of the
 *          form prime + 2 not already used which shares a factor with a(n-1).
 * https://oeis.org/A257311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is defined recursively:
   a(1) = 4; a(2) = 5.
   For n > 2, a(n) is the smallest number of the form prime + 2 not
   already used that shares a non‑trivial factor with a(n-1).  The code
   computes terms on demand, memoises them, and updates a dynamic sieve
   of primes to supply candidates.  All memory allocations are cast
   appropriately for C++ (and C). */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------------
   Helper functions
   ------------------------------------------------------------------ */

/* Greatest common divisor for values that fit in seq_t.
   Assumes seq_t defines % and > operators. */
static seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b) {
        seq_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* Simple Sieve of Eratosthenes: generates all primes <= limit.
   The caller owns *primes_p and *cnt_p and must free them.          */
static void sieve_primes(size_t limit, uint64_t **primes_p, size_t *cnt_p)
{
    bool *comp = (bool *) calloc(limit + 1U, sizeof(bool));
    if (!comp) return;               /* failure – caller will not use data */

    comp[0] = comp[1] = true;
    for (size_t i = 2; i * i <= limit; ++i) {
        if (!comp[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                comp[j] = true;
        }
    }

    /* Collect primes */
    size_t est = limit / 8 + 10; /* rough upper bound */
    uint64_t *pr = (uint64_t *) malloc(est * sizeof(uint64_t));
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!comp[i]) {
            if (cnt == est) {
                est <<= 1;
                pr = (uint64_t *) realloc(pr, est * sizeof(uint64_t));
            }
            pr[cnt++] = i;
        }
    }
    pr = (uint64_t *) realloc(pr, cnt * sizeof(uint64_t));

    free(comp);
    *primes_p = pr;
    *cnt_p    = cnt;
}

/* Extend the prime table to at least new_limit (doubling strategy).
   Keeps the used flags for previously seen primes.             */
static void extend_primes(size_t new_limit, uint64_t **primes,
                          size_t *cnt, bool **used, size_t *used_cap,
                          size_t *sieve_limit)
{
    if (new_limit <= *sieve_limit) return;

    uint64_t *new_primes = NULL;
    size_t new_cnt = 0;
    sieve_primes(new_limit, &new_primes, &new_cnt);

    bool *new_used = (bool *) calloc(new_cnt, sizeof(bool));
    for (size_t i = 0; i < *cnt; ++i) new_used[i] = (*used)[i];

    free(*primes);
    free(*used);
    *primes = new_primes;
    *cnt = new_cnt;
    *used = new_used;
    *used_cap = new_cnt;
    *sieve_limit = new_limit;
}

/* ------------------------------------------------------------------
   Sequence generation
   ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    /* Initial constant terms */
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)5;

    /* Static state for memoisation */
    static seq_t *seq = NULL;        /* 1‑based array of terms */
    static size_t seq_cap = 0;       /* allocated capacity */
    static idx_t  seq_len = 0;       /* highest index computed */

    /* Static state for primes */
    static uint64_t *primes = NULL; /* list of primes */
    static size_t   primes_cnt = 0; /* number of primes stored */
    static bool    *used = NULL;    /* used[prime_index] */
    static size_t  used_cap = 0;     /* capacity of used array */
    static size_t  sieve_limit = 200000; /* current prime table limit */

    /* Ensure prime table big enough for the requested index */
    if (primes_cnt < (size_t)n) {
        /* Double the limit until enough primes */
        while (primes_cnt < (size_t)n) {
            extend_primes(sieve_limit * 2 + 100, &primes, &primes_cnt,
                          &used, &used_cap, &sieve_limit);
        }
    }

    /* Allocate/resize sequence array */
    if (!seq || (size_t)n + 1 > seq_cap) {
        seq = (seq_t *) realloc(seq, ((size_t)n + 1) * sizeof(seq_t));
        seq_cap = (size_t)n + 1;
    }

    /* Initialise first two terms and mark their primes as used */
    if (seq_len < 2) {
        seq[1] = (seq_t)4;   /* prime 2 */
        seq[2] = (seq_t)5;   /* prime 3 */
        used[0] = true;      /* prime 2 used */
        used[1] = true;      /* prime 3 used */
        seq_len = 2;
    }

    /* Return already computed values */
    if (seq_len >= n) return seq[n];

    /* Generate terms up to n */
    for (idx_t idx = seq_len + 1; idx <= n; ++idx) {
        const seq_t prev = seq[idx - 1];
        seq_t chosen = 0;
        size_t chosen_i = SIZE_MAX;

        /* Find the smallest unused prime+2 sharing a factor with prev */
        for (size_t i = 0; i < primes_cnt; ++i) {
            if (used[i]) continue;
            const seq_t candidate = (seq_t)primes[i] + (seq_t)2;
            if (gcd_seq(prev, candidate) > (seq_t)1) {
                chosen = candidate;
                chosen_i = i;
                break;
            }
        }

        /* In the unlikely event no prime found, extend the sieve */
        while (chosen == 0) {
            extend_primes(sieve_limit * 2 + 100, &primes, &primes_cnt,
                          &used, &used_cap, &sieve_limit);
            for (size_t i = 0; i < primes_cnt; ++i) {
                if (used[i]) continue;
                const seq_t candidate = (seq_t)primes[i] + (seq_t)2;
                if (gcd_seq(prev, candidate) > (seq_t)1) {
                    chosen = candidate;
                    chosen_i = i;
                    break;
                }
            }
        }

        used[chosen_i] = true;
        seq[idx] = chosen;
    }

    seq_len = n;
    return seq[n];
}
