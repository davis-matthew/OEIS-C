/*
 * A343758  Total area of all p X r rectangles, where n = p + r, p <= r, p is
 *          prime and r is a positive integer.
 * https://oeis.org/A343758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p prime, p <= floor(n/2)} p*(n-p)
 * Range:     u8 n <= 20, u16 n <= 145, u32 n <= 7344, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static state for prime storage */
static bool *sieve_arr = NULL;         /* Sieve of Eratosthenes array */
static uint64_t *prime_list = NULL;    /* Array of primes */
static size_t prime_cnt = 0;           /* Number of primes stored */
static idx_t sieve_limit = 0;          /* Highest index sieved so far */

/* Rebuild the sieve and prime list to include all primes up to 'limit' */
static void ensure_primes(idx_t limit)
{
    if (limit <= sieve_limit) return;               /* Already sufficient */

    /* Free old data */
    free(sieve_arr);
    free(prime_list);
    sieve_arr = NULL;
    prime_list = NULL;
    prime_cnt = 0;
    sieve_limit = 0;

    if (limit < 2) return;                          /* No primes needed */

    /* Allocate sieve array: indices 0..limit */
    sieve_arr = (bool*)calloc((size_t)limit + 1, sizeof(bool));
    if (!sieve_arr) return;                         /* Allocation failed */

    /* Mark all numbers as prime initially */
    for (idx_t i = 0; i <= limit; ++i) sieve_arr[i] = true;
    sieve_arr[0] = sieve_arr[1] = false;

    /* Eratosthenes sieve */
    for (idx_t p = 2; p * p <= limit; ++p)
    {
        if (sieve_arr[p])
        {
            for (idx_t m = p * p; m <= limit; m += p)
                sieve_arr[m] = false;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (sieve_arr[i]) ++cnt;

    /* Allocate prime list if any primes */
    if (cnt > 0)
    {
        prime_list = (uint64_t*)malloc(cnt * sizeof(uint64_t));
        if (prime_list)
        {
            size_t idx = 0;
            for (idx_t i = 2; i <= limit; ++i)
                if (sieve_arr[i]) prime_list[idx++] = (uint64_t)i;
        }
    }

    prime_cnt |= cnt;
    sieve_limit = limit;
}

/* Sequence a(n) */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Compute up to the largest prime needed */
    idx_t half = n >> 1;          /* floor(n/2) */
    if (half >= 2)
        ensure_primes(half);
    else
        return (seq_t)0;          /* No primes to consider */

    seq_t sum = 0;
    for (size_t i = 0; i < prime_cnt && prime_list[i] <= half; ++i)
    {
        idx_t p = (idx_t)prime_list[i];
        sum += (seq_t)p * (seq_t)(n - p);
    }
    return sum;
}
