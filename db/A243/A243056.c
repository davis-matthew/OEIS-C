/*
 * A243056  If n is the i-th prime, p_i = A000040(i), then a(n) = i, otherwise
 *          the difference between the indices of the smallest and the largest
 *          prime dividing n: for n = p_i * ... * p_k, where p_i <= ... <=
 *          p_k, a(n) = (k-i); a(1) = 0 by convention.
 * https://oeis.org/A243056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1618, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Function implementing OEIS A243056:
   a(1) = 0
   if n = p_i (the i‑th prime) then a(n) = i
   otherwise a(n) = index( largest prime factor of n ) -
                    index( smallest prime factor of n )
*/

#include <stdlib.h>
#include <stdint.h>

/* The types idx_t (unsigned 64‑bit) and seq_t (unsigned, any size)
   are defined by the build system.  We only use them, we do not
   redefine them. */

static idx_t sieve_limit = 0;          /* largest n for which primes are known */
static uint64_t *prime_list = NULL;    /* list of primes */
static size_t prime_count  = 0;        /* number of primes found */
static idx_t *prime_index  = NULL;     /* prime_index[p] = 1‑based index of prime p, 0 if not prime */

/* Build or extend the sieve up to at least n.  The implementation
   reallocates the arrays anew for simplicity; this is fine for n
   up to the thousands required by the test harness. */
static void ensure_primes(idx_t n)
{
    if (n <= sieve_limit) return;

    /* free old tables if they exist */
    free(prime_list);
    free(prime_index);

    idx_t limit = n;
    prime_list   = (uint64_t *)malloc((size_t)limit * sizeof(uint64_t));
    prime_index  = (idx_t *)calloc((size_t)limit + 1, sizeof(idx_t));
    if (!prime_list || !prime_index) exit(EXIT_FAILURE);

    char *is_composite = (char *)calloc((size_t)limit + 1, 1);
    if (!is_composite) exit(EXIT_FAILURE);

    prime_count = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_composite[i]) {
            prime_list[prime_count++] = i;
            prime_index[i] = (idx_t)(prime_count);   /* 1‑based index */

            if (i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i) {
                    is_composite[j] = 1;
                }
            }
        }
    }

    free(is_composite);
    sieve_limit = limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)0;

    ensure_primes(n);

    /* If n itself is prime, return its index directly */
    if (prime_index[(size_t)n] != (idx_t)0) {
        return (seq_t)prime_index[(size_t)n];
    }

    /* Composite case: find smallest and largest prime factors */
    idx_t tmp = n;
    uint64_t spf = 0;  /* smallest prime factor */
    uint64_t lpf = 0;  /* largest prime factor */

    size_t i;
    for (i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > tmp) break;
        if (tmp % p == 0) {
            if (spf == 0) spf = p;
            lpf = p;
            while (tmp % p == 0) tmp /= p;
        }
    }

    /* If a prime factor larger than sqrt(n) remains */
    if (tmp > 1) {
        if (spf == 0) spf = tmp;
        lpf = tmp;
    }

    /* Indices of smallest and largest prime factors */
    idx_t idx_spf = prime_index[(size_t)spf];
    idx_t idx_lpf = prime_index[(size_t)lpf];

    return (seq_t)(idx_lpf - idx_spf);
}
