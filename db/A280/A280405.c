/*
 * A280405  Odd semiprimes that cannot be represented as 2p+3q, where p and q
 *          are primes.
 * https://oeis.org/A280405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*p where p is the n-th odd prime such that p == 3 or p-2
 *            is not prime (i.e., p-2 is composite). The values returned are
 *            the odd semiprimes 3p which cannot be expressed as 2q + 3r with
 *            q,r primes
 * Range:     u8 n <= 14, u16 n <= 2077, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

static bool *sieve_arr = NULL;          /* sieve bitmap (true = prime)   */
static size_t sieve_size = 0;           /* current allocated size        */

/* Build a sieve of Eratosthenes up to 'limit' inclusive.
   The array is reallocated only when a larger limit is required. */
static void build_sieve(size_t limit)
{
    if (sieve_arr && sieve_size > limit) return;  /* sufficient already */

    free(sieve_arr);
    sieve_arr = (bool *)malloc(limit + 1);
    if (!sieve_arr) return;  /* allocation failure not handled */

    for (size_t i = 0; i <= limit; ++i) sieve_arr[i] = true;
    if (limit >= 0) sieve_arr[0] = false;
    if (limit >= 1) sieve_arr[1] = false;

    for (size_t i = 2; i * i <= limit; ++i) {
        if (sieve_arr[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                sieve_arr[j] = false;
        }
    }
    sieve_size = limit + 1;
}

/* Return the nth term of the sequence, counting from n = 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0, return 0 */

    /* Estimate a limit large enough to contain the nth qualifying prime.
       Use a simple upper bound for the nth prime: n(log n + log log n) + 3n. */
    uint64_t limit;
    if (n < 6) {
        limit = 15;
    } else {
        double nd = (double)n;
        double estimate = nd * (log(nd) + log(log(nd)));
        estimate += 3.0 * nd;   /* safety margin */
        limit = (uint64_t)(estimate) + 10;
    }

    /* Keep extending the sieve until we find the nth term. */
    while (true) {
        build_sieve(limit);

        idx_t found = 0;
        for (uint64_t p = 3; p <= limit; p += 2) {
            if (!sieve_arr[p]) continue;              /* not prime */
            if (p == 3 || !sieve_arr[p - 2]) {         /* qualifying prime */
                ++found;
                if (found == n)
                    return (seq_t)3 * (seq_t)p;       /* 3p is the desired value */
            }
        }

        /* Not enough primes: double the limit and try again. */
        limit *= 2;
    }
}
