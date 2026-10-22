/*
 * A347799  Number of compositions (ordered partitions) of n into at most 6
 *          nonprime parts.
 * https://oeis.org/A347799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of compositions of n into at most 6 nonprime parts
 * Range:     u8 n <= 18, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static storage for the computed sequence and auxiliary tables. */
static seq_t *sequence = NULL;      /* a[n] values */
static idx_t  sequence_size = 0;    /* maximum n for which a() is ready */

/* Simple sieve of Eratosthenes returning all primes ≤ limit. */
static void sieve_primes(idx_t limit, idx_t **primes_out, size_t *cnt_out)
{
    size_t sz = (size_t)(limit + 1);
    bool *is_prime = (bool *)calloc(sz, sizeof(bool));
    if (!is_prime) {
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }

    for (size_t i = 2; i < sz; ++i) is_prime[i] = true;

    size_t capacity = limit / 2 + 1;               /* rough upper bound */
    idx_t  *primes  = (idx_t *)malloc(capacity * sizeof(idx_t));
    size_t  count   = 0;

    for (size_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            if (count + 1 > capacity) {
                capacity *= 2;
                primes = (idx_t *)realloc(primes, capacity * sizeof(idx_t));
            }
            primes[count++] = (idx_t)i;

            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    is_prime[j] = false;
            }
        }
    }

    free(is_prime);
    *primes_out = primes;
    *cnt_out    = count;
}

/* Recompute the sequence up to new_limit (inclusive). */
static void recompute_up_to(idx_t new_limit)
{
    size_t sz = (size_t)(new_limit + 1);
    seq_t *a          = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *coeff_prev = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *coeff_curr = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *pref       = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!a || !coeff_prev || !coeff_curr || !pref) return;

    /* build prime list */
    idx_t *prime_list = NULL;
    size_t prime_cnt  = 0;
    sieve_primes(new_limit, &prime_list, &prime_cnt);

    /* Base: only composition with 0 parts for n==0 */
    coeff_prev[0] = (seq_t)1;
    a[0]          = (seq_t)1;

    /* iterate for k = 1 .. 6 */
    for (int k = 1; k <= 6; ++k) {
        /* prefix sums of coeff_prev (all parts allowed) */
        pref[0] = coeff_prev[0];
        for (size_t i = 1; i < sz; ++i)
            pref[i] = pref[i-1] + coeff_prev[i];

        /* start with all parts allowed */
        coeff_curr[0] = (seq_t)0;
        for (size_t n = 1; n < sz; ++n)
            coeff_curr[n] = pref[n-1];

        /* subtract contributions where the last part is a prime */
        for (size_t pi = 0; pi < prime_cnt; ++pi) {
            idx_t p = prime_list[pi];
            for (size_t n = p; n < sz; ++n)
                coeff_curr[n] -= coeff_prev[n - p];
        }

        /* accumulate into final sequence (compositions up to k parts) */
        for (size_t n = 0; n < sz; ++n)
            a[n] += coeff_curr[n];

        /* prepare for next iteration */
        seq_t *tmp = coeff_prev;
        coeff_prev = coeff_curr;
        coeff_curr = tmp;
    }

    /* free previous sequence if any and store new one */
    free(sequence);
    sequence = a;
    sequence_size = new_limit;

    /* free temporary arrays */
    free(coeff_prev);
    free(coeff_curr);
    free(pref);
    free(prime_list);
}

seq_t a(idx_t n)
{
    if (n > sequence_size || sequence == NULL)
        recompute_up_to(n);
    return sequence[(size_t)n];
}
