/*
 * A241566  Number of 2-element subsets of {1,...,n} whose sum has more than 2
 *          divisors.
 * https://oeis.org/A241566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # { (i,j) | 1 ≤ i < j ≤ n, i + j composite }
 * Range:     u8 n <= 27, u16 n <= 396, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation counts all 2‑element subsets of {1,…,n} and
   subtracts those whose sum is prime.  It uses a simple Sieve of
   Eratosthenes to generate all primes up to 2·max_n seen so far,
   then for each prime p counts how many pairs (i,j) add up to p.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* Static storage for the prime sieve. */
static uint64_t *primes = NULL;      /* array of primes */
static size_t   prime_count = 0;     /* number of primes stored       */
static idx_t    prime_limit = 0;     /* maximum n*2 that has been
                                       sieved so far                 */

/* Generate all primes up to 'limit' and store them in the static
   array 'primes'.  The sieve is recomputed from scratch each time
   it is called with a larger limit. */
static void generate_primes(idx_t limit)
{
    size_t i, j, count;
    idx_t  sqrt_limit = (idx_t)sqrt((double)limit);
    
    /* Allocate and initialise the sieve array. */
    uint8_t *sieve = (uint8_t *)malloc((size_t)(limit + 1));
    if (sieve == NULL) { return; }
    for (i = 0; i <= (size_t)limit; ++i) sieve[i] = 1;
    if (limit >= 0) sieve[0] = 0;
    if (limit >= 1) sieve[1] = 0;

    /* Classic Sieve of Eratosthenes. */
    for (i = 2; i <= sqrt_limit; ++i) {
        if (sieve[i]) {
            for (j = i * i; j <= (size_t)limit; j += i) {
                sieve[j] = 0;
            }
        }
    }

    /* Count primes and allocate the primes array. */
    count = 0;
    for (i = 2; i <= (size_t)limit; ++i)
        if (sieve[i]) ++count;

    /* Realloc primes array to hold all primes. */
    uint64_t *new_primes = (uint64_t *)realloc(primes, count * sizeof(uint64_t));
    if (new_primes == NULL && count > 0) {
        free(sieve);
        return;
    }
    primes = new_primes;
    prime_count = count;

    /* Store primes into the array. */
    size_t idx = 0;
    for (i = 2; i <= (size_t)limit; ++i) {
        if (sieve[i]) {
            primes[idx++] = (uint64_t)i;
        }
    }

    /* Clean up the sieve array. */
    free(sieve);
    prime_limit = limit;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;   /* no subsets for n = 1 */

    /* Ensure we have primes up to 2*n. */
    idx_t needed = n + n;
    if (needed > prime_limit) {
        generate_primes(needed);
    }

    seq_t total_pairs = ((seq_t)n) * (((seq_t)n) - 1) / 2;
    seq_t prime_pairs = 0;

    /* Count pairs whose sum is a prime. */
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = primes[i];
        if (p > needed) break;

        idx_t low  = (p > n) ? (p - n) : 1;     /* i >= max(1, p - n)   */
        idx_t high = (p - 1) / 2;               /* i <= floor((p-1)/2)  */

        if (high >= low) {
            prime_pairs += ((seq_t)high - (seq_t)low + 1);
        }
    }

    return total_pairs - prime_pairs;
}
