/*
 * A258934  Half the difference between the 2n-th prime and the n-th prime,
 *          starting from n=2.
 * https://oeis.org/A258934
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (prime(2*n) - prime(n)) / 2
 * Range:     u8 n <= 27, u16 n <= 3346, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/sieve_first_n_primes.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 2.  For smaller n we return 0
       (the behaviour below the offset is irrelevant). */
    if (n < 2) return (seq_t)0;

    /* Number of primes we need: 2*n (0‑based indices 0..2*n‑1). */
    size_t need = 2 * (size_t)n;

    /* Static cache of primes.  The array is allocated only once and
       enlarged when necessary. */
    static size_t primes_alloc = 0;
    static uint32_t *primes = NULL;

    if (primes_alloc < need) {
        /* Allocate new array of size 'need'.  The function
           sieve_first_n_primes allocates its own array and returns
           it via the second argument.  We replace the old array
           with the new one after freeing it. */
        size_t cnt;
        uint32_t *tmp = NULL;
        sieve_first_n_primes(need, &tmp, &cnt);
        if (tmp == NULL) abort();  /* allocation failure */

        free(primes);
        primes = tmp;
        primes_alloc = cnt;  /* cnt == need */
    }

    /* Indexes in the array are 0‑based: prime(n) -> primes[n-1] */
    seq_t p_n   = (seq_t)primes[n - 1];
    seq_t p_2n  = (seq_t)primes[2 * n - 1];

    return (p_2n - p_n) / 2;
}
