/*
 * A343782  Sum of the positive differences of the prime pairs (p,q) such that
 *          n = p + q and p <= q.
 * https://oeis.org/A343782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..floor(n/2)} c(i) * c(n-i) * (n-2*i)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here c(k) is 1 if k is prime, 0 otherwise.  We compute the sum of
   positive differences for all prime pairs (p, q) with p + q = n and p <= q. */
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    /* For n < 5 there are no prime pairs with p ≤ q, return 0. */
    if (n < 5u) {
        return (seq_t)0;
    }

    /* Sieve of Eratosthenes up to n.  We use a dynamically allocated
       array of bools; uint8_t is sufficient. */
    size_t limit = (size_t)n;
    bool *sieve = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!sieve) {
        return (seq_t)0;  /* Insufficient memory – unlikely for the test ranges. */
    }
    memset(sieve, 0, (limit + 1) * sizeof(bool));

    for (size_t i = 2; i * i <= limit; ++i) {
        if (!sieve[i]) {
            for (size_t j = i * i; j <= limit; j += i) {
                sieve[j] = true;
            }
        }
    }

    seq_t sum = 0;
    /* Iterate over primes p <= n/2 and check if q = n - p is prime. */
    for (size_t p = 2; p <= n / 2; ++p) {
        if (!sieve[p]) { /* p is prime */
            size_t q = n - p;
            if (!sieve[q]) { /* q is also prime, and p <= q by construction */
                sum += (seq_t)(q - p);
            }
        }
    }

    free(sieve);
    return sum;
}
