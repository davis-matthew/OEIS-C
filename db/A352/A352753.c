/*
 * A352753  a(n) = (pi(2n-1) - pi(n-1)) * Sum_{p <= n, p prime} p.
 * https://oeis.org/A352753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (pi(2n-1) - pi(n-1)) * (sum of all primes p <= n)
 * Range:     u8 n <= 16, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* seq_t and idx_t are defined by the build environment. */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;         /* defensive; spec starts at n=1 */

    /* limit for the sieve: 2n-1 */
    idx_t limit = 2 * n - 1;
    /* Allocate sieve and prefix sums.  Cast calloc return to the correct types. */
    unsigned char *is_prime = (unsigned char *)calloc((size_t)(limit + 1), 1);
    idx_t *prime_cnt = (idx_t *)calloc((size_t)(limit + 1), sizeof(idx_t));
    seq_t *prime_sum = (seq_t *)calloc((size_t)(limit + 1), sizeof(seq_t));
    if (!is_prime || !prime_cnt || !prime_sum) {
        free(is_prime); free(prime_cnt); free(prime_sum);
        return (seq_t)0;
    }

    /* Initialize sieve: mark all numbers ≥2 as potential primes */
    for (idx_t i = 2; i <= limit; ++i) is_prime[i] = 1;

    /* Eratosthenes sieve */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i) {
                is_prime[j] = 0;
            }
        }
    }

    /* Compute cumulative counts and sums of primes */
    prime_cnt[0] = 0;
    prime_sum[0] = (seq_t)0;
    if (limit >= 1) {
        prime_cnt[1] = 0;
        prime_sum[1] = (seq_t)0;
    }
    for (idx_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            prime_cnt[i] = prime_cnt[i - 1] + 1;
            prime_sum[i] = prime_sum[i - 1] + (seq_t)i;
        } else {
            prime_cnt[i] = prime_cnt[i - 1];
            prime_sum[i] = prime_sum[i - 1];
        }
    }

    /* Pi difference: pi(2n-1) - pi(n-1) */
    idx_t pi_diff = prime_cnt[limit] - prime_cnt[n - 1];
    /* Sum of primes <= n */
    seq_t sum_primes = prime_sum[n];

    seq_t result = (seq_t)pi_diff * sum_primes;

    /* Clean up */
    free(is_prime);
    free(prime_cnt);
    free(prime_sum);

    return result;
}
