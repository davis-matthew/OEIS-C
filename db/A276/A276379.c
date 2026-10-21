/*
 * A276379  Write a "1" for each distinct prime divisor p of n in the (pi(p) -
 *          1)-th place, ignoring multiplicity.
 * https://oeis.org/A276379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = decimal number whose digit at position (π(p)-1) *
 *            (counted from the most-significant digit) is 1 iff the prime
 *            divisor p * divides n, otherwise 0. * Equivalently: a(n) =
 *            A054841(A007947(n)), written in base 10 with * all non‑zero
 *            digits replaced by 1
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 30, u64 n <= 72, u128 n <= 172,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* Static storage for the prime list */
static uint64_t *prime_list   = NULL;   /* primes ≤ prime_limit */
static size_t   prime_count  = 0;       /* number of primes in prime_list   */
static idx_t    prime_limit  = 0;       /* current upper bound of prime_list */

/* Build a sieve up to 'limit' and fill prime_list and prime_count.  */
static void build_primes_upto(idx_t limit)
{
    if (limit < 2) {
        if (prime_list) free(prime_list);
        prime_list = NULL;
        prime_count = 0;
        prime_limit = limit;
        return;
    }

    /* Allocate a boolean marker array on the heap.  */
    bool *is_prime = (bool*)calloc((size_t)limit + 1U, sizeof(bool));
    if (!is_prime) { /* allocation failure – leave primes empty */
        if (prime_list) free(prime_list);
        prime_list = NULL;
        prime_count = 0;
        prime_limit = limit;
        return;
    }

    /* Sieve of Eratosthenes */
    for (size_t i = 2; i <= (size_t)limit; ++i) is_prime[i] = true;
    for (size_t i = 2; i * i <= (size_t)limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= (size_t)limit; j += i)
                is_prime[j] = false;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i)
        if (is_prime[i]) ++cnt;

    /* Allocate and store primes */
    uint64_t *primes = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!primes) {
        free(is_prime);
        if (prime_list) free(prime_list);
        prime_list = NULL;
        prime_count = 0;
        prime_limit = limit;
        return;
    }

    size_t idx = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i)
        if (is_prime[i]) primes[idx++] = (uint64_t)i;

    /* Replace old prime list */
    free(prime_list);
    prime_list = primes;
    prime_count = cnt;
    prime_limit = limit;

    free(is_prime);
}

/* Ensure we have all primes up to 'limit', enlarging the list if needed */
static void ensure_primes(idx_t limit)
{
    if (limit > prime_limit)
        build_primes_upto(limit);
}

/* Main entry point for the sequence. */
seq_t a(idx_t n)
{
    if (n <= 1)  /* by definition a(1)=0, a(0) is undefined */
        return (seq_t)0;

    /* Prepare primes up to n, needed for π(p) of any prime divisor. */
    ensure_primes(n);

    /* Allocate a flag array for primes dividing n. */
    bool *has = (bool*)calloc(prime_count, sizeof(bool));
    if (!has)  /* allocation failure – unlikely in tests */
        return (seq_t)0;

    uint64_t temp = (uint64_t)n;
    /* Trial division using the prime list. */
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p > temp / p) break;            /* p*p > temp */
        if (temp % p == 0) {
            has[i] = true;
            while (temp % p == 0) temp /= p;
        }
    }
    /* If a prime factor remains greater than sqrt(original n). */
    if (temp > 1) {
        /* Binary search for this remaining prime inside the prime list. */
        size_t lo = 0, hi = prime_count;
        while (lo < hi) {
            size_t mid = (lo + hi) >> 1;
            if (prime_list[mid] < temp) lo = mid + 1;
            else hi = mid;
        }
        if (lo < prime_count && prime_list[lo] == temp)
            has[lo] = true;
    }

    /* Determine the largest index with a '1'. */
    size_t max_idx = 0;
    for (size_t i = 0; i < prime_count; ++i)
        if (has[i]) max_idx = i;

    /* Build the decimal number digit by digit from most‑significant
     * (largest prime index) down to 0. */
    seq_t value = 0;
    for (size_t i = max_idx; i != (size_t)-1; --i) { /* loop descending */
        value = value * (seq_t)10 + (has[i] ? (seq_t)1 : (seq_t)0);
        if (i == 0) break;   /* avoid underflow when i reaches 0 */
    }

    free(has);
    return value;
}
