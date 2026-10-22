/*
 * A305801  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => f(i) = f(j), where f(n) = 0 if n is an odd prime, with f(n) = n
 *          for all other n.
 * https://oeis.org/A305801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=2, a(n)=3 if n>2 is an odd prime, a(n)=n-π(n)+2 for
 *            composite n, where π(n) is the number of primes ≤ n. This yields
 *            the lexicographically earliest sequence satisfying: a(i)=a(j) ⇒
 *            f(i)=f(j) with f(n)=0 for odd primes and f(n)=n otherwise
 * Range:     u8 n <= 319, u16 n <= 8004, u32 n <= 8004, u64 n <= 8004,
 *            u128 n <= 8004, big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime.h"          /* bool is_prime(idx_t n); */
#include "../utils/build_primes_upto.h" /* void build_primes_upto(idx_t limit, uint64_t **array, size_t *cnt); */

static uint64_t *sieve_primes = NULL;   /* array of primes up to sieve_limit */
static size_t sieve_count = 0;          /* number of primes in ->sieve_primes */
static idx_t sieve_limit = 0;           /* current maximum n sieved */

static void ensure_primes_up_to(idx_t n) {
    if (n <= sieve_limit)
        return;
    /* free old array */
    free(sieve_primes);
    sieve_primes = NULL;
    sieve_count = 0;
    sieve_limit = 0;
    /* build primes up to n */
    build_primes_upto(n, &sieve_primes, &sieve_count);
    sieve_limit = n;
}

static size_t prime_count_up_to(idx_t n) {
    size_t c = 0;
    for (size_t i = 0; i < sieve_count; ++i) {
        if (sieve_primes[i] > n)
            break;
        ++c;
    }
    return c;
}

seq_t a(idx_t n) {
    if (n == 1)
        return 1;
    if (n == 2)
        return 2;
    if ((n & 1) && is_prime(n))
        return 3;                 /* all odd primes share the value 3 */

    /* composite n > 2 */
    ensure_primes_up_to(n);
    size_t pi = prime_count_up_to(n);   /* π(n) includes the prime 2 */
    return (seq_t)n - (seq_t)pi + 2;
}
