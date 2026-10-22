/*
 * A334208  Number of partitions of 2n into two composite parts, (r,s), such
 *          that r and s have the same number of primes less than or equal to
 *          them.
 * https://oeis.org/A334208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{i=2..n} [pi(i)=pi(2*n-i)] * (1 - c(i)) * (1 -
 *            c(2*n-i))
 * Range:     u8 n <= 9592, u16 n <= 9592, u32 n <= 9592, u64 n <= 9592,
 *            u128 n <= 9592, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes by sieving primes up to 2*n and counting composite partitions with equal pi. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Persistent sieve data.  They are enlarged only when a larger limit is needed. */
static bool *prime_flag   = NULL;   /* prime_flag[i] == true  <=>  i is prime          */
static uint32_t *pi_arr   = NULL;   /* pi_arr[i]   = pi(i)                           */
static idx_t     max_limit = 0;     /* maximum index for which arrays are built     */

/* Rebuild or enlarge the sieve up to 'limit' (inclusive). */
static void rebuild_sieve(idx_t limit)
{
    /* Allocate or extend the arrays if the requested limit exceeds the current one. */
    if (limit > max_limit) {
        bool *pf = (bool*)realloc(prime_flag, (size_t)(limit + 1) * sizeof(bool));
        if (!pf) exit(1);
        prime_flag = pf;

        uint32_t *pi = (uint32_t*)realloc(pi_arr, (size_t)(limit + 1) * sizeof(uint32_t));
        if (!pi) exit(1);
        pi_arr = pi;

        /* Initialise all entries as prime, then sieve. */
        for (size_t i = 0; i <= (size_t)limit; ++i)
            prime_flag[i] = true;
        if (limit >= 0) {
            prime_flag[0] = false;
            if (limit >= 1) prime_flag[1] = false;
        }

        for (idx_t p = 2; p <= limit / p; ++p) {
            if (prime_flag[p]) {
                for (idx_t m = p * p; m <= limit; m += p)
                    prime_flag[m] = false;
            }
        }

        /* Build the cumulative prime counts. */
        uint32_t cnt = 0;
        pi_arr[0] = 0;
        for (idx_t i = 1; i <= limit; ++i) {
            if (prime_flag[i]) ++cnt;
            pi_arr[i] = cnt;
        }

        max_limit = limit;
    }
}

/* Sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* offset allows a(1) ... */

    idx_t limit = n + n;                 /* 2*n, safe for given bounds */
    rebuild_sieve(limit);                /* ensure arrays exist up to 2*n */

    seq_t result = 0;
    for (idx_t i = 2; i <= n; ++i) {
        idx_t j = limit - i;             /* 2*n - i */

        if (prime_flag[i]) continue;     /* i must be composite */
        if (prime_flag[j]) continue;     /* j must be composite */

        if (pi_arr[i] == pi_arr[j])     /* equal number of primes ≤ each part */
            ++result;
    }
    return result;
}
