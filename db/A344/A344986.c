/*
 * A344986  Number of Goldbach partitions of 2n into 2 primes where the
 *          smaller prime has an even-index and the larger prime has an odd
 *          index.
 * https://oeis.org/A344986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of Goldbach partitions of 2n into two primes where
 *            the smaller prime has an even index (2,4,6,…) and the larger
 *            prime has an odd index (1,3,5,…)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ------------------------------------------------------------------
   Sieve cache: all data for numbers up to `current_limit`.  All
   numbers are stored as 0‑based indices in the arrays except
   `prime_index_arr`, which is 1‑based (prime_index_arr[p] == prime
   position of prime p, 0 if p is composite).
   ------------------------------------------------------------------ */
static size_t current_limit = 3;          /* minimal limit (covers 2,3) */
static uint8_t *is_prime = NULL;         /* 1 if prime, 0 otherwise */
static uint32_t *prime_index_arr = NULL; /* 1‑based prime index, 0 if composite */
static uint32_t *primes = NULL;          /* ordered list of primes */
static size_t primes_count = 0;          /* number of primes stored */

/* ------------------------------------------------------------------
   Ensure that all integers <= limit are sieved.  The function
   rebuilds the whole cache if `limit` is larger than the current one.
   ------------------------------------------------------------------ */
static void ensure_limit(idx_t limit)
{
    if (limit <= (idx_t)current_limit) {
        return;
    }

    size_t new_limit = (size_t)limit;

    /* Allocate new sieve arrays */
    uint8_t *new_is_prime = (uint8_t *)calloc(new_limit + 1, 1);
    if (!new_is_prime) abort();

    uint32_t *new_prime_index_arr = (uint32_t *)calloc(new_limit + 1, sizeof(uint32_t));
    if (!new_prime_index_arr) abort();

    uint32_t *new_primes = (uint32_t *)calloc(new_limit + 1, sizeof(uint32_t));
    if (!new_primes) abort();

    /* Simple Eratosthenes sieve */
    for (size_t i = 2; i <= new_limit; ++i) new_is_prime[i] = 1;
    for (size_t p = 2; p * p <= new_limit; ++p) {
        if (new_is_prime[p]) {
            for (size_t m = p * p; m <= new_limit; m += p) {
                new_is_prime[m] = 0;
            }
        }
    }

    /* Build prime list and index array */
    size_t pc = 0;
    for (size_t i = 2; i <= new_limit; ++i) {
        if (new_is_prime[i]) {
            new_prime_index_arr[i] = (uint32_t)(pc + 1);  /* 1‑based index */
            new_primes[pc++] = (uint32_t)i;
        }
    }

    /* Free old cache */
    free(is_prime);
    free(prime_index_arr);
    free(primes);

    /* Install new cache */
    is_prime = new_is_prime;
    prime_index_arr = new_prime_index_arr;
    primes = new_primes;
    primes_count = pc;
    current_limit = new_limit;
}

/* ------------------------------------------------------------------
   Main sequence function.  It is thread‑unsafe because of the static
   sieve cache, but the exercise does not require thread safety.
   ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Need primes up to 2*n for Goldbach partitions */
    ensure_limit((idx_t)(2 * n));

    seq_t count = (seq_t)0;

    /* Iterate over all primes p <= n (smaller prime in the partition) */
    for (size_t i = 0; i < primes_count; ++i) {
        uint32_t p = primes[i];
        if (p > (uint32_t)n) break;              /* primes > n are not allowed */

        uint32_t q = (uint32_t)(2 * n - p);       /* partner prime */
        if (q < p) continue;                     /* keep unordered pairs */

        if (q <= (uint32_t)current_limit && is_prime[q]) {
            uint32_t idx_p = prime_index_arr[p];
            uint32_t idx_q = prime_index_arr[q];
            if ((idx_p & 1U) == 0 && (idx_q & 1U) == 1U) {
                ++count;
            }
        }
    }

    return count;
}
