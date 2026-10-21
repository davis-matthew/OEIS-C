/*
 * A294345  Sum of the products of the smaller and larger parts of the
 *          Goldbach partitions of n into two distinct parts.
 * https://oeis.org/A294345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=2..⌊(n-1)/2⌋} i*(n-i) for all i, n-i that are
 *            prime. * (Only distinct prime Goldbach partitions contribute;
 *            equal primes are excluded.)
 * Range:     u8 n <= 23, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Cached prime sieve.  prime_flags[i] == true iff i is prime. */
static bool *prime_flags = NULL;
static idx_t prime_limit = 0;

/* Build a prime sieve up to at least limit.  Allocates/reallocates as needed. */
static void
ensure_primes(idx_t limit)
{
    if (limit <= prime_limit && prime_flags != NULL)
        return;                     /* already ready */

    /* Allocate new sieve */
    bool *new_flags = (bool *)malloc((size_t)(limit + 1) * sizeof(bool));
    if (!new_flags)
        return;                     /* allocation failed – leave old sieve intact */

    /* Initialize all entries to true, then clear 0 and 1. */
    for (size_t i = 0; i <= (size_t)limit; ++i)
        new_flags[i] = true;
    if (limit >= 0) new_flags[0] = false;
    if (limit >= 1) new_flags[1] = false;

    /* Sieve of Eratosthenes */
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (new_flags[p]) {
            for (idx_t m = p * p; m <= limit; m += p)
                new_flags[m] = false;
        }
    }

    /* Replace old sieve */
    free(prime_flags);
    prime_flags = new_flags;
    prime_limit = limit;
}

seq_t
a(idx_t n)
{
    if (n < 1)
        return 0;                     /* outside the offset – defined as 0 */

    ensure_primes(n);                 /* sieve up to n */

    idx_t upper = (n - 1) / 2;        /* last i to consider (ensures i < n-i) */
    seq_t sum = 0;

    for (idx_t i = 2; i <= upper; ++i) {
        if (prime_flags[i] && prime_flags[n - i]) {
            sum += (seq_t)i * (seq_t)(n - i);
        }
    }
    return sum;
}
