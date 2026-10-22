/*
 * A391840  Base 16 sum of digits of prime(n).
 * https://oeis.org/A391840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of base-16 digits of the n-th prime
 * Range:     u8 n <= 54, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function requires the build_primes utility, which generates
   all primes up to a given limit.  The primes are cached between
   calls so that repeated evaluations are fast.  The function
   computes the sum of hexadecimal digits of the desired prime
   and returns that value as a seq_t. */

#include <stddef.h>
#include "../utils/build_primes.h"

static idx_t *primes = NULL;
static size_t primes_cnt = 0;
static idx_t sieve_limit = 0;

static inline idx_t estimate_limit(idx_t n)
{
    /* Rough upper bound for the n-th prime:
       p_n < n*(log n + log log n) for n >= 6.
       A simple linear overestimate (20*n + 100) is sufficient
       for the typical ranges (n up to ~10^5). */
    if (n < 6) return 15;
    return n * 20 + 100;
}

seq_t a(idx_t n)
{
    /* Ensure we have at least n primes cached. */
    if (n == 0) return 0;          /* undefined for n=0, return 0 */
    if (primes_cnt < n || sieve_limit < estimate_limit(n)) {
        /* Need more primes – rebuild the sieve up to a larger limit. */
        idx_t new_limit = estimate_limit(n);
        idx_t *new_primes = NULL;
        size_t new_cnt = 0;

        build_primes(new_limit, &new_primes, &new_cnt);

        /* Replace old cache. */
        free(primes);
        primes = new_primes;
        primes_cnt = new_cnt;
        sieve_limit = new_limit;
    }

    /* Retrieve the n-th prime (indexing starts at 1). */
    seq_t p = (seq_t)primes[n - 1];

    /* Sum of hexadecimal digits of p. */
    seq_t sum = 0;
    while (p != (seq_t)0) {
        seq_t digit = p & (seq_t)0xF;	/* least significant hex digit */
        sum += digit;
        p >>= 4;                       /* shift right by 4 bits */
    }
    return sum;
}
