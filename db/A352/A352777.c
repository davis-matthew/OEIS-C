/*
 * A352777  a(n) = Sum_{p <= n <= q < 2n, p,q prime} (p * q).
 * https://oeis.org/A352777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (sum_{p <= n, p prime} p) * (sum_{q in [n,2n-1], q prime}
 *            q)
 * Range:     u8 n <= 6, u16 n <= 35, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static unsigned char *sieve_isprime = NULL;
static seq_t *cum_sum = NULL;
static idx_t sieve_limit = 0;

/* Build sieve up to maximum_limit and compute cumulative sum of primes */
static void build_sieve(idx_t maximum_limit)
{
    idx_t i, j;
    unsigned char *isprime = (unsigned char*)malloc((maximum_limit + 1) * sizeof(unsigned char));
    seq_t *cumsum = (seq_t*)malloc((maximum_limit + 1) * sizeof(seq_t));
    if (!isprime || !cumsum) {
        /* Allocation failure – release any allocated memory */
        free(isprime); free(cumsum);
        return;
    }

    /* Initialize all as prime candidates */
    for (i = 0; i <= maximum_limit; ++i) isprime[i] = 1;
    if (maximum_limit >= 0) isprime[0] = 0;
    if (maximum_limit >= 1) isprime[1] = 0;

    /* Sieve */
    for (i = 2; i * i <= maximum_limit; ++i) {
        if (isprime[i]) {
            for (j = i * i; j <= maximum_limit; j += i) {
                isprime[j] = 0;
            }
        }
    }

    /* Compute cumulative sum of primes */
    cumsum[0] = 0;
    for (i = 1; i <= maximum_limit; ++i) {
        if (isprime[i]) {
            cumsum[i] = cumsum[i - 1] + (seq_t)i;
        } else {
            cumsum[i] = cumsum[i - 1];
        }
    }

    /* Replace old data with new */
    free(sieve_isprime);
    free(cum_sum);
    sieve_isprime = isprime;
    cum_sum = cumsum;
    sieve_limit = maximum_limit;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for n < 1, but return 0 */

    /* Ensure sieve is built up to 2*n - 1 */
    idx_t required_limit = 2 * n - 1;
    if (required_limit > sieve_limit) {
        build_sieve(required_limit);
    }

    /* sum of primes <= n */
    seq_t sum_left = cum_sum[n];

    /* sum of primes in [n, 2n-1] = cum_sum[2n-1] - cum_sum[n-1] */
    seq_t sum_right = cum_sum[required_limit] - cum_sum[n - 1];

    return sum_left * sum_right;
}
