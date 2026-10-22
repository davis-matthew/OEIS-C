/*
 * A363074  Prime numbers that are the exact average of two consecutive odd
 *          semiprimes.
 * https://oeis.org/A363074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime numbers that are the exact average of two
 *            consecutive odd semiprimes
 * Range:     u8 n <= 16, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* global data */
static idx_t           sieve_limit = 0;          /* maximum index up to which sieve is built   */
static uint32_t *spf = NULL;                    /* smallest prime factor for each number      */
static uint8_t *is_prime = NULL;                /* true if number is prime                   */

static seq_t *results = NULL;                   /* array of discovered terms                 */
static size_t results_cnt = 0;
static size_t results_cap = 0;

static idx_t  next_odd = 9;                     /* next odd number to examine                */
static seq_t  last_semiprime = 0;               /* most recent odd semiprime                 */

/* ------------------------------------------------------------------------- */
/* Build a linear sieve up to 'limit' and fill spf[] and is_prime[].          */
/* ------------------------------------------------------------------------- */
static void build_sieve(idx_t limit)
{
    /* free old data */
    free(spf);
    free(is_prime);

    /* allocate */
    spf = (uint32_t *)calloc((size_t)(limit + 1), sizeof(uint32_t));
    is_prime = (uint8_t *)calloc((size_t)(limit + 1), sizeof(uint8_t));

    /* linear sieve */
    size_t primes_cap = limit / 10 + 10;           /* rough upper bound   */
    uint32_t *prime_list = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
    size_t primes_cnt = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {                        /* i is prime */
            spf[i] = (uint32_t)i;
            if (primes_cnt == primes_cap) {
                primes_cap <<= 1;
                prime_list = (uint32_t *)realloc(prime_list, primes_cap * sizeof(uint32_t));
            }
            prime_list[primes_cnt++] = (uint32_t)i;
        }
        for (size_t k = 0; k < primes_cnt &&
                         (uint64_t)prime_list[k] <= spf[i] &&
                         (uint64_t)i * (uint64_t)prime_list[k] <= limit; ++k) {
            spf[i * (idx_t)prime_list[k]] = prime_list[k];
        }
    }

    /* fill prime flag */
    for (idx_t i = 2; i <= limit; ++i) {
        is_prime[i] = (spf[i] == (uint32_t)i);
    }
    free(prime_list);

    sieve_limit = limit;
}

/* ------------------------------------------------------------------------- */
/* Append a new term to the results array.                                   */
/* ------------------------------------------------------------------------- */
static void append_result(seq_t val)
{
    if (results_cap == 0) {
        results_cap = 8;
        results = (seq_t *)malloc(results_cap * sizeof(seq_t));
    } else if (results_cnt == results_cap) {
        results_cap <<= 1;
        results = (seq_t *)realloc(results, results_cap * sizeof(seq_t));
    }
    results[results_cnt++] = val;
}

/* ------------------------------------------------------------------------- */
/* Main function: a(n)                                                         */
/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        /* initial sieve; enough for the first 10000 terms */
        build_sieve(20000000);
        next_odd = 9;
        last_semiprime = 0;
        initialized = true;
    }

    while (results_cnt < n) {
        /* extend sieve if needed */
        if (next_odd > sieve_limit) {
            idx_t new_limit = sieve_limit * 2;
            if (new_limit < next_odd * 2)
                new_limit = next_odd * 2;
            build_sieve(new_limit);
        }

        idx_t curr = next_odd;
        next_odd += 2;                           /* advance to next odd */

        if (curr > sieve_limit) continue;        /* safety check */

        /* is curr an odd semiprime? */
        if (spf[curr] != 0 && spf[curr] != curr && is_prime[curr / spf[curr]]) {
            if (last_semiprime != 0) {
                seq_t avg = ((seq_t)last_semiprime + (seq_t)curr) / 2;
                if (is_prime[(idx_t)avg]) {
                    append_result(avg);
                }
            }
            last_semiprime = (seq_t)curr;
        }
    }

    return results[n - 1];
}
