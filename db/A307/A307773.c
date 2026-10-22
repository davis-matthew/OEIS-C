/*
 * A307773  Expansion of e.g.f. Sum_{k>=1} prime(k)*log(1/(1 - x))^k/k!.
 * https://oeis.org/A307773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} |Stirling1(n,k)| * prime(k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses recurrence for unsigned Stirling numbers of the first kind
 * and a simple sieve-like primality test to obtain primes. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"  /* provides extern bool is_prime(idx_t); */

 /* Cache for computed a(n) values */
static seq_t *cached_vals   = NULL;  /* indices 1..max_n   */
static size_t cached_cap    = 0;
static idx_t  max_cached_n  = 0;

/* Rows for Stirling numbers (c(n,k)) */
static seq_t *row_prev      = NULL;  /* previous row */
static seq_t *row_cur       = NULL;  /* current row  */
static size_t row_cap       = 0;

/* Prime list cache */
static uint64_t *prime_arr   = NULL; /* primes[0] = 2, primes[k-1] = prime(k) */
static size_t   prime_cap    = 0;
static size_t   prime_cnt    = 0;

/* Ensure that prime_arr contains at least n primes. */
static void ensure_primes(size_t n_needed)
{
    if (prime_cnt >= n_needed)
        return;

    /* Grow array if necessary */
    if (prime_cap < n_needed) {
        size_t new_cap = prime_cap ? prime_cap * 2 : n_needed * 2;
        uint64_t *tmp = (uint64_t *)realloc(prime_arr, new_cap * sizeof(uint64_t));
        prime_arr = tmp;
        prime_cap = new_cap;
    }

    idx_t cand = (idx_t)(prime_cnt ? prime_arr[prime_cnt - 1] + 1 : 2);
    while (prime_cnt < n_needed) {
        if (is_prime(cand))
            prime_arr[prime_cnt++] = cand;
        cand++;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1, undefined for n < 1 */
        return (seq_t)0;

    /* Grow cached_vals if needed */
    if (cached_cap < (size_t)(n + 1)) {
        seq_t *tmp = (seq_t *)realloc(cached_vals, (n + 1) * sizeof(seq_t));
        cached_vals = tmp;
        cached_cap  = (size_t)(n + 1);
    }

    /* Grow Stirling rows if needed */
    if (row_cap < (size_t)(n + 1)) {
        seq_t *tmp1 = (seq_t *)realloc(row_prev, (n + 1) * sizeof(seq_t));
        seq_t *tmp2 = (seq_t *)realloc(row_cur,  (n + 1) * sizeof(seq_t));
        row_prev = tmp1;
        row_cur  = tmp2;
        row_cap  = (size_t)(n + 1);
    }

    /* Ensure we have enough primes for this n */
    ensure_primes((size_t)n);

    /* Compute values up to n if not already done */
    if (max_cached_n < n) {
        /* Initialize first row n=1 */
        if (max_cached_n == 0) {
            row_prev[0] = (seq_t)0;
            row_prev[1] = (seq_t)1;            /* c(1,1)=1 */
            cached_vals[1] = row_prev[1] * (seq_t)prime_arr[0];
            max_cached_n = 1;
        }

        /* Compute subsequent rows */
        for (idx_t i = max_cached_n + 1; i <= n; ++i) {
            row_cur[0] = (seq_t)0;
            /* Compute for k=1..i-1 */
            for (idx_t k = 1; k < i; ++k) {
                seq_t term1 = row_prev[k - 1];
                seq_t term2 = row_prev[k] * (seq_t)(i - 1);
                row_cur[k] = term1 + term2;
            }
            row_cur[i] = (seq_t)1;              /* c(i,i)=1 */

            /* Accumulate the sum for a(i) */
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= i; ++k) {
                sum += row_cur[k] * (seq_t)prime_arr[k - 1];
            }
            cached_vals[i] = sum;

            /* Prepare for next iteration */
            seq_t *tmp = row_prev;
            row_prev = row_cur;
            row_cur  = tmp;
        }
        max_cached_n = n;
    }

    return cached_vals[n];
}
