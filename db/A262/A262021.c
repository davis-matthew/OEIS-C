/*
 * A262021  a(n) = prime(prime(n)) - n.
 * https://oeis.org/A262021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(prime(n)) - n
 * Range:     u8 n <= 16, u16 n <= 855, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Estimate an upper bound for the nth prime (valid for n >= 6) */
static idx_t estimate_prime_limit(idx_t n)
{
    if (n < 6) return 15;
    double dn = (double)n;
    double est = dn * (log(dn) + log(log(dn)));
    return (idx_t)(est) + 10;        /* add a small safety margin */
}

/* Generate all primes up to 'limit' (inclusive).
   Returned array is stored in *primes_out, length in *cnt_out. */
static void generate_primes(idx_t limit, uint64_t **primes_out, size_t *cnt_out)
{
    /* sieve array: 0 = prime, 1 = composite */
    char *sieve = (char *)malloc((size_t)(limit + 1));
    if (!sieve) return;              /* allocation failed – ignore, caller
                                         will get empty list */
    memset(sieve, 0, (size_t)(limit + 1));

    size_t capacity = limit / 10;    /* rough initial capacity */
    if (capacity == 0) capacity = 1;
    uint64_t *primes = (uint64_t *)malloc(capacity * sizeof(uint64_t));
    if (!primes) { free(sieve); return; }

    size_t count = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!sieve[i]) {                       /* i is prime */
            if (count == capacity) {           /* grow storage */
                capacity *= 2;
                uint64_t *tmp = (uint64_t *)realloc(primes, capacity * sizeof(uint64_t));
                if (!tmp) { free(primes); free(sieve); return; }
                primes = tmp;
            }
            primes[count++] = (uint64_t)i;

            if (i * i <= limit) {              /* mark multiples */
                for (idx_t j = i * i; j <= limit; j += i)
                    sieve[j] = 1;
            }
        }
    }

    free(sieve);
    *primes_out = primes;
    *cnt_out = count;
}

/* Main function computing the sequence */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n < 1, but guard */

    /* ---------- First sieve: find p_n = prime(n) ---------- */
    idx_t limit_n = estimate_prime_limit(n);
    uint64_t *primes_n = NULL;
    size_t cnt_n = 0;
    generate_primes(limit_n, &primes_n, &cnt_n);

    /* If not enough primes, enlarge and retry (unlikely for small n) */
    while (cnt_n < n) {
        free(primes_n);
        limit_n = (limit_n * 2) + 100;
        generate_primes(limit_n, &primes_n, &cnt_n);
    }

    uint64_t p_n = primes_n[n - 1];   /* prime(n) */

    /* ---------- Second sieve: find p_{p_n} = prime(prime(n)) ---------- */
    idx_t limit_m = estimate_prime_limit(p_n);
    uint64_t *primes_m = NULL;
    size_t cnt_m = 0;
    generate_primes(limit_m, &primes_m, &cnt_m);

    /* Ensure we have p_n primes */
    while (cnt_m < p_n) {
        free(primes_m);
        limit_m = (limit_m * 2) + 100;
        generate_primes(limit_m, &primes_m, &cnt_m);
    }

    uint64_t p_pn = primes_m[p_n - 1];   /* prime(prime(n)) */

    /* Clean up */
    free(primes_n);
    free(primes_m);

    /* Compute a(n) = p_{p_n} - n */
    seq_t result = (seq_t)p_pn - (seq_t)n;
    return result;
}
