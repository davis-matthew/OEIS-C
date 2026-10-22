/*
 * A338363  a(n) = n + pi(n) - pi(floor(n/2)), where pi = A000720.
 * https://oeis.org/A338363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + pi(n) - pi(floor(n/2))
 * Range:     u8 n <= 234, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

static idx_t max_limit = 0;      /* highest index we've sieved up to */
static seq_t *pi_counts = NULL;  /* pi_counts[i] = number of primes ≤ i */

/* Build a prime sieve up to 'limit' and compute pi_counts[0..limit] */
static void sieve_up_to(idx_t limit)
{
    /* allocate array of bools for the sieve (cast for C++) */
    bool *prime_arr = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!prime_arr) abort();
    for (idx_t i = 0; i <= limit; ++i) prime_arr[i] = true;
    if (limit >= 0) prime_arr[0] = false;
    if (limit >= 1) prime_arr[1] = false;

    /* sieve */
    for (idx_t p = 2; (uint64_t)p * p <= limit; ++p) {
        if (prime_arr[p]) {
            for (idx_t m = p * p; m <= limit; m += p)
                prime_arr[m] = false;
        }
    }

    /* allocate array for pi counts (cast for C++) */
    seq_t *pi_arr = (seq_t *)malloc((limit + 1) * sizeof(seq_t));
    if (!pi_arr) abort();

    seq_t cnt = 0;
    for (idx_t i = 0; i <= limit; ++i) {
        if (prime_arr[i]) ++cnt;
        pi_arr[i] = cnt;
    }

    /* free old data */
    free(pi_counts);
    pi_counts = pi_arr;
    max_limit = limit;

    /* we no longer need the prime array */
    free(prime_arr);
}

/* Ensure we have a sieve and pi_counts up to 'n' */
static inline void ensure_up_to(idx_t n)
{
    if (n <= max_limit) return;
    sieve_up_to(n);
}

seq_t a(idx_t n)
{
    /* off‑offset safety – a(0) is not defined by OEIS but we avoid crash */
    if (n == 0) return 0;

    ensure_up_to(n);

    seq_t pi_n    = pi_counts[n];
    const idx_t half = n / 2;
    seq_t pi_half = pi_counts[half];

    return (seq_t)n + pi_n - pi_half;
}
