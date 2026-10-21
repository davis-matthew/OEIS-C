/*
 * A271233  Composite integers sandwiched between primes p, q with q-p = 8.
 * https://oeis.org/A271233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A271233: Composite integers sandwiched between consecutive primes
   p and q with q - p = 8.
   A(n) is the n-th composite number x such that there exist two consecutive
   primes p < x < q with q - p = 8. */

/*  Implementation: generate primes sequentially by trial division.
    For each pair of consecutive primes, if the gap is 8 we produce all
    intermediate integers (which are composite by definition).
    We count them until we reach the desired index n. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Test whether n is prime using the list of primes found so far. */
static bool is_prime_u64(uint64_t n, uint64_t *primes, size_t primesCnt)
{
    for (size_t i = 0; i < primesCnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > n) {
            break;
        }
        if (n % p == 0) {
            return false;
        }
    }
    return true;
}

/* External function required by the framework. */
seq_t a(idx_t n)
{
    /* Allocate an array for primes discovered so far. */
    size_t primesCap = 16;
    uint64_t *primes = (uint64_t *)malloc(primesCap * sizeof(uint64_t));
    if (!primes) {
        /* In case of allocation failure, return 0 (should not happen). */
        return (seq_t)0;
    }

    /* Seed with the first prime. */
    primes[0] = 2;
    size_t primesCnt = 1;
    uint64_t prevPrime = 2;
    uint64_t candidate = 3;            /* next odd number to test */
    unsigned long long counter = 0;    /* number of composites found */

    for (;;) {
        if (is_prime_u64(candidate, primes, primesCnt)) {
            /* Store the new prime.  Grow array if necessary. */
            if (primesCnt >= primesCap) {
                primesCap <<= 1;
                uint64_t *tmp = (uint64_t *)realloc(primes, primesCap * sizeof(uint64_t));
                if (!tmp) {
                    free(primes);
                    return (seq_t)0;
                }
                primes = tmp;
            }
            primes[primesCnt++] = candidate;

            if (candidate - prevPrime == 8ULL) {
                /* All numbers strictly between prevPrime and candidate
                   are composite (by definition of twin primes). */
                for (uint64_t x = prevPrime + 1; x < candidate; ++x) {
                    ++counter;
                    if (counter == n) {
                        free(primes);
                        return (seq_t)x;
                    }
                }
            }
            prevPrime = candidate;
        }
        candidate += 2;   /* skip even numbers */
    }
}
