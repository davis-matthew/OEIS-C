/*
 * A347550  Number of partitions of n into at most 2 distinct prime parts.
 * https://oeis.org/A347550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is prime else 0 + the number of unordered distinct
 *            prime pairs summing to n (i.e., partitions of n into at most two
 *            distinct prime parts)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* a(0) = 1, a(1) = 0 by definition */
    if (n == 0)   return (seq_t)1;
    if (n < 2)    return (seq_t)0;

    /* Allocate a sieve array of size n+1.  The cast to (bool*) is required for valid C++ code. */
    size_t sz = (size_t)n + 1;
    bool *prime = (bool*)calloc(sz, sizeof(bool));
    if (!prime)  return (seq_t)0;   /* allocation failure */

    /* Initialise all candidates >= 2 as prime */
    for (idx_t i = 2; i <= n; ++i)
        prime[(size_t)i] = true;

    /* Eratosthenes sieve */
    for (idx_t i = 2; i * i <= n; ++i) {
        if (prime[(size_t)i]) {
            for (idx_t j = i * i; j <= n; j += i)
                prime[(size_t)j] = false;
        }
    }

    /* Count partitions: at most two distinct prime parts */
    seq_t count = 0;

    /* One‑part partition: n itself prime */
    if (prime[(size_t)n]) ++count;

    /* Two‑part partitions: unordered pairs of distinct primes p < q with p + q = n */
    for (idx_t p = 2; p <= n / 2; ++p) {
        if (prime[(size_t)p] && prime[(size_t)(n - p)] && p < n - p)
            ++count;
    }

    free(prime);
    return count;
}
