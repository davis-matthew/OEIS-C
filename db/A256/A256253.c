/*
 * A256253  Number of successive odd nonprimes A014076 and number of
 *          successive odd primes A065091, interleaved.
 * https://oeis.org/A256253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A256253: Number of successive odd nonprimes and primes alternated.
   a(n) is the length of the n‑th run of consecutive odd numbers that are
   either all nonprimes (odd rows) or all primes (even rows).  The first odd
   number 1 is treated as a nonprime. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* ---------- Prime sieve helpers ---------- */
static uint32_t *primes      = NULL;   /* primes list (ascending)          */
static size_t   primes_len   = 0;
static size_t   primes_cap   = 0;
static uint32_t max_prime    = 0;      /* largest prime stored so far      */

/* Ensure that primes up to LIMIT (inclusive) are available. */
static void
ensure_primes_up_to(uint32_t limit)
{
    if (limit <= max_prime)
        return;

    /* First allocation */
    if (primes_len == 0) {
        primes_cap   = 16;
        primes       = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
        primes[0]    = 2;
        primes_len   = 1;
        max_prime    = 2;
    }

    /* Start from the first candidate greater than max_prime */
    uint32_t cand = max_prime + 1;
    if (cand <= 2)
        cand = 3;          /* start with first odd candidate */
    if (cand % 2 == 0)
        cand++;            /* make it odd */

    for (; cand <= limit; cand += 2) {
        bool is_prime = true;
        uint32_t sqrt_cand = (uint32_t)sqrt((double)cand);
        for (size_t i = 0; i < primes_len; ++i) {
            uint32_t p = primes[i];
            if (p > sqrt_cand)
                break;
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (primes_len == primes_cap) {
                primes_cap *= 2;
                primes = (uint32_t *)realloc(primes,
                    primes_cap * sizeof(uint32_t));
            }
            primes[primes_len++] = cand;
            max_prime = cand;
        }
    }
}

/* Test whether an odd integer n (≥3) is prime. */
static bool
is_prime_odd(uint64_t n)
{
    if (n < 2)
        return false;
    if (n == 2)
        return true; /* never called for even n */

    uint32_t sqrt_n = (uint32_t)sqrt((double)n);
    ensure_primes_up_to(sqrt_n);

    for (size_t i = 0; i < primes_len; ++i) {
        uint64_t p = primes[i];
        if (p * p > n)
            break;
        if (n % p == 0)
            return false;
    }
    return true;
}

/* ---------- Sequence construction helpers ---------- */
static bool            initialized    = false;
static uint64_t        current_odd    = 1;  /* next odd number to inspect */
static bool            cur_type_prime = false; /* false -> non‑prime row */
static size_t          cur_run_len    = 0; /* length of current run */

/* Cached run lengths */
static seq_t *runs      = NULL;
static size_t runs_len  = 0;
static size_t runs_cap  = 0;

/* Ensure that run lengths for indices up to target_n are computed. */
static void
ensure_runs_up_to(idx_t target_n)
{
    if (!initialized) {
        initialized = true;
        current_odd = 1;
        cur_type_prime = false;
        cur_run_len = 0;

        runs_cap = 16;
        runs = (seq_t *)malloc(runs_cap * sizeof(seq_t));
        runs_len = 0;
    }

    while (runs_len < (size_t)target_n) {
        bool prime = is_prime_odd(current_odd);
        if (prime == cur_type_prime) {
            ++cur_run_len;
        } else {
            if (runs_len == runs_cap) {
                runs_cap *= 2;
                runs = (seq_t *)realloc(runs,
                    runs_cap * sizeof(seq_t));
            }
            runs[runs_len++] = (seq_t)cur_run_len;
            cur_type_prime = !cur_type_prime;
            cur_run_len = 1;     /* current odd counted as first of new run */
        }
        current_odd += 2;
    }
}

/* ---------- Main function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* below offset is irrelevant */

    ensure_runs_up_to(n);
    return runs[n - 1];
}
