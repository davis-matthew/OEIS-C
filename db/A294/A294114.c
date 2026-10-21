/*
 * A294114  Sum of the larger parts of the partitions of 2n into two parts
 *          with smaller part prime.
 * https://oeis.org/A294114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} (2*n - i) * c(i) (c(i)=1 iff i is prime)
 * Range:     u8 n <= 20, u16 n <= 467, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h> /* for bool if needed */

static idx_t sieve_max = 0;
static uint8_t *prime_flag      = NULL;   /* 0/1 flag for primality */
static idx_t  *prime_pi        = NULL;   /* pi(n) prefix counts   */
static seq_t  *prime_sum       = NULL;   /* sum of primes prefix  */

/* Rebuild the sieve and prefix arrays up to at least n. */
static void ensure_sieve(idx_t n)
{
    if (n <= sieve_max) return;

    /* free old arrays */
    free(prime_flag);
    free(prime_pi);
    free(prime_sum);

    sieve_max = n;
    prime_flag = (uint8_t *)malloc((n + 1) * sizeof(uint8_t));
    prime_pi   = (idx_t  *)malloc((n + 1) * sizeof(idx_t));
    prime_sum  = (seq_t  *)malloc((n + 1) * sizeof(seq_t));

    if (!prime_flag || !prime_pi || !prime_sum) return; /* out of memory */

    /* initially assume all numbers are prime except 0 and 1 */
    prime_flag[0] = prime_flag[1] = 0;
    for (idx_t i = 2; i <= n; ++i) prime_flag[i] = 1;

    /* simple Eratosthenes sieve */
    for (idx_t i = 2; i * i <= n; ++i) {
        if (prime_flag[i]) {
            for (idx_t j = i * i; j <= n; j += i) {
                prime_flag[j] = 0;
            }
        }
    }

    /* build prefix sums */
    prime_pi[0] = 0;
    prime_sum[0] = 0;
    for (idx_t i = 1; i <= n; ++i) {
        prime_pi[i] = prime_pi[i-1] + prime_flag[i];
        prime_sum[i] = prime_sum[i-1] + ((seq_t)prime_flag[i] * (seq_t)i);
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    ensure_sieve(n);

    /* pi(n) is the count of primes <= n */
    idx_t pi_n = prime_pi[n];

    /* sum of primes <= n */
    seq_t sum_p = prime_sum[n];

    /* two_n = 2*n as seq_t */
    seq_t two_n = (seq_t)n + (seq_t)n;

    /* a(n) = 2*n*pi(n) - sum_of_primes */
    seq_t res = two_n * (seq_t)pi_n - sum_p;
    return res;
}
