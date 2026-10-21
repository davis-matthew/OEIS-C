/*
 * A294108  Minimum of the number of primes appearing among the smaller parts
 *          and the number of primes appearing among the larger parts of the
 *          partitions of n into two parts.
 * https://oeis.org/A294108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = min( π(floor(n/2)), π(n-1) – π(ceil(n/2)−1) ) where π(x)
 *            is the number of primes ≤ x
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static storage for the sieve and the prime-count array */
static idx_t   sieve_limit = 0;
static uint8_t *prime_flag = NULL;   /* 0/1 flags for primality */
static seq_t  *pi_arr     = NULL;   /* π(i) for i = 0 .. limit */

/* recompute sieve and π array up to at least 'limit' */
static void recompute_sieve(idx_t limit)
{
    /* free previous storage if any */
    if (prime_flag) free(prime_flag);
    if (pi_arr)     free(pi_arr);

    /* allocate with size limit+1 (including index 0) */
    prime_flag = (uint8_t *)malloc((limit + 1) * sizeof(uint8_t));
    pi_arr     = (seq_t *)malloc((limit + 1) * sizeof(seq_t));
    if (!prime_flag || !pi_arr) {
        /* allocation failed: make the pointers NULL to avoid use */
        if (prime_flag) free(prime_flag);
        if (pi_arr) free(pi_arr);
        prime_flag = NULL;
        pi_arr = NULL;
        sieve_limit = 0;
        return;
    }

    /* initialise all entries as potential primes */
    for (idx_t i = 0; i <= limit; ++i) prime_flag[i] = 1;
    if (limit >= 0) prime_flag[0] = 0;
    if (limit >= 1) prime_flag[1] = 0;

    /* classic sieve of Eratosthenes */
    idx_t sq = (idx_t)((double)limit * 0.5 + 1.0); /* safe upper bound */
    for (idx_t p = 2; p <= sq; ++p) {
        if (prime_flag[p]) {
            for (idx_t j = p * p; j <= limit; j += p)
                prime_flag[j] = 0;
        }
    }

    /* build π(i) array */
    pi_arr[0] = 0;
    for (idx_t i = 1; i <= limit; ++i) {
        pi_arr[i] = pi_arr[i-1] + (seq_t)(prime_flag[i] != 0);
    }

    sieve_limit = limit;
}

/* ensure sieve data is available up to 'need' (inclusive) */
static void ensure_sieve(idx_t need)
{
    if (need <= sieve_limit) return;
    recompute_sieve(need);
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n=0, return 0 */

    /* We need primes up to n-1 at most. */
    idx_t limit = (n == 0) ? 0 : n - 1;
    ensure_sieve(limit);
    if (!pi_arr) return (seq_t)0;          /* allocation failed */

    /* count primes among smaller parts: primes ≤ floor(n/2) */
    idx_t small_end = n / 2;               /* floor(n/2) */
    seq_t count_small = pi_arr[small_end];

    /* count primes among larger parts: primes in [ceil(n/2), n-1] */
    idx_t large_start = (n + 1) / 2;       /* ceil(n/2) */
    seq_t count_large = 0;
    if (large_start <= limit) {
        /* π(limit) = π(n-1) */
        seq_t pi_limit = pi_arr[limit];
        seq_t pi_before = (large_start == 0) ? 0 : pi_arr[large_start - 1];
        count_large = pi_limit - pi_before;
    }

    return (count_small <= count_large) ? count_small : count_large;
}
