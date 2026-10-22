/*
 * A379948  Positive integers k such that k*(tau(k) - 1)/(sigma(k) - k/d) is
 *          an integer for at least one divisor d of k.
 * https://oeis.org/A379948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 706, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, factor_u64, sigma_of, test_divisors_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A379948: numbers k such that k*(tau(k)-1)/(sigma(k)-k/d) is an integer
 * for at least one divisor d of k
 *
 * This implementation tests each integer k ≥ 2, generates all its divisors
 * by recursive enumeration, and checks the divisibility condition.
 * The sequence values are cached in a dynamically sized array so that
 * calls to a(idx_t n) are O(n) overall (one-time work per term),
 * and the function works for any idx_t/seq_t combination that the build
 * supplies.  No hard‑coded terms are used. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>     /* malloc, realloc, free */

/* Utility functions from the OEIS utilities library */
#include "../utils/factor_u64.h"
#include "../utils/divisor_count_u64.h"
#include "../utils/sigma_of.h"
#include "../utils/test_divisors_rec.h"

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/*  Check whether a single integer k satisfies the sequence condition */
static bool k_is_valid(uint64_t k)
{
    if (k <= 1U)
        return false;

    seq_t tau_seq  = divisor_count_u64(k);        /* tau(k) */
    if (tau_seq <= (seq_t)1)
        return false;                             /* tau(k) - 1 == 0 */

    seq_t sigma = sigma_of(k);                    /* σ(k) */
    seq_t numerator = (seq_t)k * (tau_seq - (seq_t)1);

    /* Factorisation of k */
    uint64_t primes[64], exps[64];
    size_t pcnt;
    factor_u64(k, primes, exps, &pcnt);

    return test_divisors_rec(primes, exps, pcnt, 0, 1,
                             sigma, numerator, k);
}

/* ------------------------------------------------------------------ */
/*  Static storage for the generated sequence */
static seq_t  *seq_vals  = NULL;  /* array of terms */
static size_t  seq_len   = 0;     /* number of stored terms */
static size_t  seq_cap   = 0;     /* capacity of seq_vals   */
static uint64_t last_k   = 2;     /* next integer to test  */

/* Ensure that seq_vals can hold at least 'need' elements. */
static void ensure_seq_cap(size_t need)
{
    if (need <= seq_cap)
        return;
    size_t new_cap = seq_cap ? (seq_cap << 1) : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
    seq_vals = tmp;
    seq_cap  = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Main function required by the test harness.  Returns the n-th term
 *  (1‑indexed) of A379948. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined, but safe */

    /* Initialise storage on first call */
    if (seq_vals == NULL) {
        seq_cap = 16;
        seq_vals = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_len  = 0;
        last_k   = 2;
    }

    /* Generate terms until we have at least n of them */
    while (seq_len < (size_t)n) {
        if (k_is_valid(last_k)) {
            ensure_seq_cap(seq_len + 1);
            seq_vals[seq_len++] = last_k;   /* implicit cast to seq_t */
        }
        ++last_k;
    }

    return seq_vals[(size_t)n - 1];
}
