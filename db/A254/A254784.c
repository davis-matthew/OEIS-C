/*
 * A254784  Apply partial sum operator 5 times to primes.
 * https://oeis.org/A254784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 94, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  a(n) = the 5th iterated partial sum of the first n primes
 *  (apply the partial sum operator 5 times to the sequence of primes).
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Compute the nth term of A254784.  The function works for any
 * seq_t that supports the usual arithmetic operators.  The
 * index type idx_t is uint64_t.  The routine allocates an
 * auxiliary array of unsigned char for the sieve; the cast is
 * legal both in C and in C++.  No floating‑point or <math.h>
 * calls are needed. */
seq_t a(idx_t n)
{
    /* Out of range requests are undefined; return 0 for safety. */
    if (n == 0)
        return (seq_t)0;

    /* Rough upper bound for the nth prime:
       for n >= 6, p_n < n (log n + log log n) < 20n for n <= 10000.
       Add a small safety margin. */
    idx_t limit;
    if (n < 6)
        limit = 16;                     /* covers p_6 = 13 */
    else
        limit = n * 20 + 100;           /* safety margin for n up to ~10^4 */

    /* Allocate the sieve array.  size_t is guaranteed to hold the size. */
    unsigned char *is_prime = (unsigned char*)malloc((size_t)(limit + 1));
    if (!is_prime)
        return (seq_t)0;                 /* allocation failure */

    /* Initialise all entries to prime; mark 0 and 1 as composite. */
    for (idx_t i = 0; i <= limit; ++i)
        is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    /* Simple Eratosthenes sieve. */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* Five successive partial sums. */
    seq_t S1 = 0, S2 = 0, S3 = 0, S4 = 0, S5 = 0;
    idx_t counted = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            ++counted;
            seq_t p = (seq_t)i;          /* the current prime */

            S1 += p;
            S2 += S1;
            S3 += S2;
            S4 += S3;
            S5 += S4;

            if (counted == n)
                break;
        }
    }

    free(is_prime);
    return S5;
}
