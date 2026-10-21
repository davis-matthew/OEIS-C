/*
 * A246669  Catalan(prime(n)).
 * https://oeis.org/A246669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Catalan(prime(n))
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include "../utils/catalan.h"

static idx_t *cached_primes = NULL;
static size_t cached_prime_count = 0;
static idx_t cached_prime_limit = 0;

/* Return the n-th prime (1‑based).  Caches primes up to the current limit. */
static idx_t get_nth_prime(idx_t n)
{
    if (n == 0) return 0;

    if ((size_t)n <= cached_prime_count)
        return cached_primes[n - 1];

    /* Estimate a limit that surely contains at least n primes.
       Use the Rosser‑Schoenfeld upper bound, plus a small margin.
       For n < 6 the bound is not valid, so we override with a small value. */
    idx_t new_limit;
    if (n < 6) {
        new_limit = 15;                     /* primes up to 15 are enough for n < 6 */
    } else {
        double dn = (double)n;
        double lim = dn * (log(dn) + log(log(dn))) + 3.0;
        new_limit = (idx_t)lim;
    }
    if (new_limit <= cached_prime_limit)
        new_limit = cached_prime_limit * 2 + 1000;
    if (new_limit < 20) new_limit = 20;

    /* Sieve of Eratosthenes up to new_limit. */
    char *sieve = (char*)calloc((size_t)new_limit + 1, 1);
    if (!sieve) return 0;

    size_t capacity = new_limit / 10 + 10;          /* initial guess for prime count  */
    idx_t *new_primes = (idx_t*)malloc(capacity * sizeof(idx_t));
    if (!new_primes) {
        free(sieve);
        return 0;
    }
    size_t count = 0;

    for (idx_t i = 2; i <= new_limit; ++i) {
        if (!sieve[i]) {
            if (count >= capacity) {
                capacity *= 2;
                idx_t *tmp = (idx_t*)realloc(new_primes, capacity * sizeof(idx_t));
                if (!tmp) {
                    free(sieve);
                    free(new_primes);
                    return 0;
                }
                new_primes = tmp;
            }
            new_primes[count++] = i;
            if (i * i <= new_limit) {
                for (idx_t j = i * i; j <= new_limit; j += i)
                    sieve[j] = 1;
            }
        }
    }
    free(sieve);

    /* Replace cached primes with the new list.  Free the old array. */
    free(cached_primes);
    cached_primes = new_primes;
    cached_prime_count = count;
    cached_prime_limit = new_limit;

    if ((size_t)n > cached_prime_count)
        return 0;                       /* should not happen */
    return cached_primes[n - 1];
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    idx_t p = get_nth_prime(n);
    return catalan(p);
}
