/*
 * A281685  Numbers k such that A001221(k) = A001221(A003415(k)).
 * https://oeis.org/A281685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A281685: numbers k such that ω(k) = ω(k') (arithmetic derivative) */
/* The function iteratively tests all integers k and stores those satisfying
   the condition; it then returns the n‑th stored value. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/count_distinct_primes.h"

/* ---------- sieve for smallest prime factors ---------- */
static idx_t sieve_limit = 0;
static uint32_t *spf      = NULL;   /* array of smallest prime factor */
static uint32_t *prime_tbl = NULL;  /* table of found primes */
static size_t   prime_cnt = 0;

/* Initialise or extend the sieve up to at least |limit|. */
static void
init_sieve(idx_t limit)
{
    if (limit <= sieve_limit)
        return;

    /* free previous tables */
    if (spf)         free(spf);
    if (prime_tbl)   free(prime_tbl);

    spf        = (uint32_t *)calloc((size_t)limit + 1, sizeof(uint32_t));
    prime_tbl = (uint32_t *)calloc((size_t)limit + 1, sizeof(uint32_t));
    prime_cnt = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            prime_tbl[prime_cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint32_t p = prime_tbl[j];
            if (p > spf[i] || (uint64_t)i * p > limit)
                break;
            spf[i * p] = p;
        }
    }
    sieve_limit = limit;
}

/* ---------- sequence storage ---------- */
static seq_t *seq_arr   = NULL;  /* dynamic array of found terms */
static size_t seq_len   = 0;     /* number of terms stored */
static size_t seq_cap   = 0;     /* allocated capacity */

/* Ensure that |seq_arr| has room for at least |need| elements. */
static void
ensure_seq_cap(size_t need)
{
    if (seq_cap < need) {
        seq_cap = (seq_cap == 0) ? 8 : seq_cap * 2;
        if (seq_cap < need)
            seq_cap = need;
        seq_arr = (seq_t *)realloc(seq_arr, seq_cap * sizeof(seq_t));
    }
}

/* ---------- main sequence function ---------- */
seq_t
a(idx_t n)
{
    /* static state to allow calls in any order */
    static idx_t current_k = 2;          /* next integer to test */
    static bool   initialized = false;

    if (!initialized) {
        /* first call initialises the storage */
        initialized = true;
        seq_len = 0;
        seq_cap = 0;
        seq_arr = NULL;
    }

    /* generate terms until we have at least n of them */
    while (seq_len < n) {
        /* make sure we have spf data up to current_k */
        if (current_k > sieve_limit)
            init_sieve(current_k * 2);

        uint64_t k_val = current_k;
        uint64_t tmp   = k_val;
        uint64_t derivative = 0;
        size_t   omega_k  = 0;   /* distinct prime factors of k */

        /* factor k and compute its arithmetic derivative */
        while (tmp > 1) {
            uint32_t p = spf[tmp];
            ++omega_k;
            uint64_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            derivative += (k_val / p) * exp;
        }

        /* distinct prime factors of the derivative   */
        int omega_deriv = count_distinct_primes(derivative);

        /* if condition holds, store the term */
        if (omega_k == (size_t)omega_deriv) {
            ensure_seq_cap(seq_len + 1);
            seq_arr[seq_len++] = (seq_t)k_val;
        }

        ++current_k;
    }

    return seq_arr[n - 1];
}
