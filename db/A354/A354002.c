/*
 * A354002  Inverse Stirling transform of odd primes.
 * https://oeis.org/A354002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{k=1..n} Stirling1(n,k) * prime(k+1)
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 20, i128 n <= 34,
 *            big any n
 * Helpers:   generate_first_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS A354002 for all n ≥ 1. */
/* Works for any seq_t / idx_t type provided by the build. */
/* Uses a simple O(n²) recurrence on the Stirling numbers of the first kind.
   Primes up to the required index are generated once by a sieve. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>            /* for log() in prime limit estimation */
#include "../utils/generate_first_primes.h"

#define MAX_CACHE 10000          /* maximum number of terms to cache */
#define SIEVE_LIMIT 200000       /* fixed sieve size for initial primes */

/* Static storage for cached terms and Stirling rows up to MAX_CACHE. */
static seq_t cache[MAX_CACHE + 1];
static seq_t row1[MAX_CACHE + 1];
static seq_t row2[MAX_CACHE + 1];
static seq_t *prev_row = row1;
static seq_t *cur_row  = row2;

/* Prime table: primes_list[1] = 2, primes_list[2] = 3, … */
static uint32_t primes_list[MAX_CACHE + 2];
static size_t primes_cnt = 0;

/* Sieve array for the initial primes. */
static bool is_prime[SIEVE_LIMIT + 1];

/* One‑time initialization flags. */
static bool initialized = false;
static idx_t max_cached = 0;

/* -------------------------------------------------------------------- */
/* Build the initial prime list up to SIEVE_LIMIT. */
static void compute_primes(void)
{
    /* Basic sieve of Eratosthenes. */
    for (idx_t i = 0; i <= SIEVE_LIMIT; ++i) is_prime[i] = false;
    for (idx_t i = 2; i <= SIEVE_LIMIT; ++i) is_prime[i] = true;
    for (idx_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= SIEVE_LIMIT; j += i)
                is_prime[j] = false;
        }
    }
    /* Store the first MAX_CACHE+1 primes. */
    for (idx_t i = 2; i <= SIEVE_LIMIT && primes_cnt < MAX_CACHE + 1; ++i) {
        if (is_prime[i]) {
            ++primes_cnt;
            primes_list[primes_cnt] = (uint32_t)i;
        }
    }
}

/* Perform one‑time initialization. */
static void init_once(void)
{
    compute_primes();
    /* Initialise Stirling row for n = 0: s(0,0)=1. */
    for (idx_t i = 0; i <= MAX_CACHE; ++i) prev_row[i] = 0;
    prev_row[0] = 1;
    initialized = true;
}

/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined, but harmless */

    if (!initialized)
        init_once();

    /* ----------------------------------------------------------------
       Handle the cached range.  The routine is fully memoised, so
       calls can be out of order. ----------------------------------- */
    if (n <= max_cached)
        return cache[n];

    /* If the requested term is beyond the cached maximum, compute
       all intermediate terms up to n.  For large n that exceed
       MAX_CACHE we use a temporary local sieve and arrays. */
    if (n > MAX_CACHE) {
        /* Generate primes up to index n+1. */
        idx_t prime_limit;
        uint32_t *primes = generate_first_primes((size_t)(n + 1), &prime_limit);

        /* Allocate Stirling rows for the current run. */
        size_t sz = (size_t)(n + 1);
        seq_t *prev = (seq_t*)calloc(sz, sizeof(seq_t));
        seq_t *cur  = (seq_t*)calloc(sz, sizeof(seq_t));
        prev[0] = 1;   /* s(0,0)=1 */

        seq_t val = 0;
        for (idx_t m = 1; m <= n; ++m) {
            for (idx_t k = 1; k <= m; ++k) {
                cur[k] = prev[k-1] - (seq_t)(m - 1) * prev[k];
            }
            cur[0] = 0;   /* s(m,0)=0 for m>0 */

            /* Sum for a(m). */
            val = 0;
            for (idx_t k = 1; k <= m; ++k)
                val += cur[k] * (seq_t)primes[k];

            /* Prepare for next iteration. */
            seq_t *tmp = prev; prev = cur; cur = tmp;
        }

        free(prev); free(cur);
        free(primes);
        return val;
    }

    /* ----------------------------------------------------------------
       Extend the cached table up to n (all values in range [max_cached+1, n]).
       The algorithm here mirrors the previous one but re‑uses the static
       arrays to keep memory usage bounded. -------------------------------- */
    for (idx_t m = max_cached + 1; m <= n; ++m) {
        for (idx_t k = 1; k <= m; ++k) {
            cur_row[k] = prev_row[k-1] - (seq_t)(m - 1) * prev_row[k];
        }
        cur_row[0] = 0;

        seq_t val = 0;
        for (idx_t k = 1; k <= m; ++k) {
            /* primes_list is indexed from 1.  prime(k+1) is primes_list[k+1]. */
            val += cur_row[k] * (seq_t)primes_list[k + 1];
        }
        cache[m] = val;

        /* swap rows for next iteration. */
        seq_t *tmp = prev_row; prev_row = cur_row; cur_row = tmp;
    }

    max_cached = n;
    return cache[n];
}
