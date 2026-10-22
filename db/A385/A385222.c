/*
 * A385222  a(n) is the multiplicative order of -10 modulo prime(n); a(1) =
 *          a(3) = 0 for completion.
 * https://oeis.org/A385222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 5036, u32 n <= 7406, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes the multiplicative order of -10 modulo the n‑th prime.
   a(n) = ord_{p_n}(-10) if p_n ≠ 2,5; otherwise 0. 
   Sequence A385222. 
   a(1) = 0, a(3) = 0 to match the OEIS offset. 
*/

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* The essence of the sequence: multiplicative order of -10 mod prime.  */
/* Helper function prototypes and data */

/* Static prime cache */
static idx_t *prime_cache = NULL;
static size_t prime_cnt = 0;

/* Ensure that at least n primes are available in prime_cache.  */
static void ensure_primes(idx_t n)
{
    if (prime_cnt >= (size_t)n)
        return;

    /* Rough upper bound for the n-th prime (safe for n ≤ 10000). */
    idx_t limit = n * 20 + 100;          /* p_10000 = 104729 < 200100 */

    size_t sieve_size = (size_t)(limit + 1);
    bool *sieve = (bool *)malloc(sieve_size);
    if (!sieve)
        return; /* allocation failure – palindrome */
    for (size_t i = 0; i < sieve_size; ++i)
        sieve[i] = false;

    for (idx_t p = 2; p * p <= limit; ++p)
    {
        if (!sieve[p]) {
            for (idx_t j = p * p; j <= limit; j += p)
                sieve[j] = true;
        }
    }

    free(prime_cache);
    prime_cache = NULL;
    prime_cnt = 0;

    for (idx_t i = 2; i <= limit; ++i)
    {
        if (!sieve[i]) {
            if (prime_cache == NULL) {
                prime_cache = (idx_t *)malloc((size_t)n * sizeof(idx_t));
                if (!prime_cache)
                    return;
            }
            prime_cache[prime_cnt++] = i;
            if (prime_cnt == (size_t)n)
                break;
        }
    }

    free(sieve);
}

/* modular exponentiation: base^exp mod mod */
static seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod)
{
    seq_t result = (seq_t)1;
    seq_t b = base % mod;
    while (exp) {
        if (exp & 1)
            result = (result * b) % mod;
        b = (b * b) % mod;
        exp >>= 1;
    }
    return result;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* outside defined range */

    /* Special values from the OEIS offset */
    if (n == 1 || n == 3)
        return (seq_t)0;

    ensure_primes(n);
    if (prime_cnt < (size_t)n)
        return (seq_t)0;         /* failure – should not happen */

    seq_t p = prime_cache[n - 1];  /* n-th prime, 1‑based index */

    /* For p = 2 or 5 the order is defined to be 0 */
    if (p == (seq_t)2 || p == (seq_t)5)
        return (seq_t)0;

    /* base = -10 mod p */
    seq_t base = p - (seq_t)(10 % p);

    /* Compute the order of 'base' modulo p */
    uint64_t order = (uint64_t)(p - 1);   /* divides p-1 */
    uint64_t tmp = order;

    /* Factorisation of p-1 (simple trial division) */
    size_t facs[32];
    size_t fc = 0;
    for (uint64_t d = 2; d * d <= tmp; ++d) {
        if (tmp % d == 0) {
            facs[fc++] = d;
            while (tmp % d == 0)
                tmp /= d;
        }
    }
    if (tmp > 1)
        facs[fc++] = tmp;

    /* Reduce the order by each prime factor */
    for (size_t i = 0; i < fc; ++i) {
        uint64_t f = facs[i];
        while (order % f == 0 && mod_pow(base, order / f, p) == (seq_t)1)
            order /= f;
    }

    return (seq_t)order;
}
