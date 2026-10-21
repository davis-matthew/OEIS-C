/*
 * A263880  Safe primes 2p + 1 such that p is a Fibonacci prime.
 * https://oeis.org/A263880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   is_prime_big, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence A263880 is the list of safe primes of the form 2p+1
   where p is a Fibonacci prime.  This implementation generates
   Fibonacci numbers, tests each for primality, and then tests the
   corresponding safe prime candidate for primality.  It caches all
   found terms so that repeated calls are fast. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* Return 0 for n < 1; behaviour below the offset is arbitrary. */
    if (n == 0)
        return (seq_t)0;

    /* Cache of safe primes found so far. */
    static seq_t *safe   = NULL;
    static size_t len    = 0;  /* number of terms cached */
    static size_t cap    = 0;  /* capacity of the cache array */

    /* Current Fibonacci numbers: F(k) and F(k+1). */
    static seq_t f_prev = (seq_t)0;
    static seq_t f_cur  = (seq_t)1;
    static bool initialized = false;

    /* Grow the cache until we have at least n items. */
    while (len < n) {
        if (!initialized) {
            initialized = true;   /* initialise only once */
        }

        /* Next Fibonacci number: F(k+2) = F(k+1) + F(k).  */
        seq_t next = f_prev + f_cur;
        f_prev = f_cur;
        f_cur  = next;

        /* Skip if the Fibonacci number is not prime. */
        if (!is_prime_big(next))
            continue;

        /* Candidate safe prime: q = 2 * p + 1.  */
        seq_t candidate = mul_small(next, 2) + (seq_t)1;

        /* Keep the candidate only if it is prime. */
        if (!is_prime_big(candidate))
            continue;

        /* Resize storage if needed.  */
        if (len == cap) {
            size_t new_cap = cap ? cap * 2 : 8;
            seq_t *tmp = (seq_t *)realloc(safe, new_cap * sizeof(seq_t));
            if (!tmp) {
                /* Allocation failed: abort (calls to this function
                   are not expected to reach this point). */
                exit(EXIT_FAILURE);
            }
            safe = tmp;
            cap  = new_cap;
        }
        safe[len++] = candidate;
    }

    return safe[n - 1];
}
