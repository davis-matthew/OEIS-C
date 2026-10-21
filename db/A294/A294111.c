/*
 * A294111  Sum of the smaller parts of the partitions of n into two parts
 *          with larger part prime.
 * https://oeis.org/A294111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{i=1}^{⌊n/2⌋} i * c(n-i) where c(x)=1 if x is prime
 *            else 0. Algorithm: sieve primes up to n with Sieve of
 *            Eratosthenes, then sum the smaller parts for which the larger
 *            part is prime
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Allocate a boolean array of size n+1 for the sieve. */
    bool *prime = NULL;
    size_t Nsize = (size_t)(n + 1U);
    if (Nsize > 0) {
        prime = (bool *)malloc(Nsize * sizeof(bool));
        if (!prime) return 0;  /* fallback on allocation failure */
        for (size_t i = 0; i < Nsize; ++i) prime[i] = true;
        if (n >= 0) prime[0] = false;
        if (n >= 1) prime[1] = false;
    }

    /* Sieve of Eratosthenes up to n. */
    for (size_t p = 2; (idx_t)p * (idx_t)p <= n; ++p) {
        if (prime[p]) {
            idx_t multiple = (idx_t)p * (idx_t)p;
            for (; multiple <= n; multiple += (idx_t)p)
                prime[multiple] = false;
        }
    }

    /* Compute the sum of smaller parts where the larger part is prime. */
    seq_t sum = 0;
    idx_t half = n >> 1U;
    for (size_t i = 1; i <= (size_t)half; ++i) {
        idx_t larger = n - (idx_t)i;
        if (larger <= n && prime[(size_t)larger])
            sum += (seq_t)(i);
    }

    free(prime);
    return sum;
}
