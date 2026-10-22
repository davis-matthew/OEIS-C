/*
 * A371981  Number of primes between two successive Sophie Germain primes,
 *          with Sophie Germain primes not themselves included in the count.
 * https://oeis.org/A371981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = π( A005384(n+1) - 1 ) - π( A005384(n) ) where π(x) is the
 *            number of primes ≤ x and A005384(n) is the nth Sophie Germain
 *            prime. This function counts the primes strictly between two
 *            successive Sophie Germain primes. It dynamically extends its
 *            sieve until it contains at least n+1 Sophie Germa...
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* static state for the sieve and precomputed data */
static idx_t sieve_limit = 0;
static uint8_t *is_prime = NULL;
static uint32_t *pi_counts = NULL;    /* prefix counts of primes */
static uint32_t *sg_arr   = NULL;    /* Sophie Germain primes */
static size_t sg_len = 0;

/* rebuild the sieve and precompute pi_counts and sg_arr up to 'limit' */
static void rebuild_sieve(idx_t limit)
{
    idx_t i, j;

    /* allocate/resize is_prime array */
    uint8_t *new_prime = (uint8_t*)malloc((size_t)(limit + 1));
    if (!new_prime) exit(1);
    for (i = 0; i <= limit; ++i) new_prime[i] = 1;
    if (limit >= 0) new_prime[0] = 0;
    if (limit >= 1) new_prime[1] = 0;

    /* Sieve of Eratosthenes */
    for (i = 2; i * i <= limit; ++i) {
        if (new_prime[i]) {
            for (j = i * i; j <= limit; j += i) new_prime[j] = 0;
        }
    }

    /* allocate/pi_counts prefix array */
    uint32_t *new_pi = (uint32_t*)malloc((size_t)(limit + 1) * sizeof(uint32_t));
    if (!new_pi) exit(1);
    new_pi[0] = 0;
    if (limit >= 1) new_pi[1] = 0;
    for (i = 2; i <= limit; ++i) {
        new_pi[i] = new_pi[i - 1] + (new_prime[i] ? 1 : 0);
    }

    /* build Sophie Germain primes list up to the limit */
    uint32_t *new_sg = (uint32_t*)malloc((size_t)(limit + 1) * sizeof(uint32_t));
    if (!new_sg) exit(1);
    size_t new_sg_len = 0;
    for (i = 2; i <= limit; ++i) {
        if (new_prime[i]) {
            uint64_t t = 2ULL * i + 1ULL;
            if (t <= limit && new_prime[(idx_t)t]) {
                new_sg[new_sg_len++] = (uint32_t)i;
            }
        }
    }

    /* free old data and install new data */
    free(is_prime);
    free(pi_counts);
    free(sg_arr);
    is_prime = new_prime;
    pi_counts = new_pi;
    sg_arr = new_sg;
    sg_len = new_sg_len;
    sieve_limit = limit;
}

/* ensure that at least n+1 Sophie Germain primes are available */
static void ensure_cover(idx_t n)
{
    if (sg_len >= (size_t)n + 1) return;

    if (!is_prime) {                      /* first initialization   */
        rebuild_sieve(1000);
    }
    while (sg_len < (size_t)n + 1) {
        idx_t new_limit = (sieve_limit > 1) ? sieve_limit * 2 : 2;
        rebuild_sieve(new_limit);
    }
}

/* the required sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* behaviour for n < 1 not needed */
    ensure_cover(n);

    idx_t low  = sg_arr[n - 1];
    idx_t high = sg_arr[n];

    /* number of primes p with low < p < high */
    seq_t result = (seq_t)(pi_counts[high - 1] - pi_counts[low]);

    return result;
}
