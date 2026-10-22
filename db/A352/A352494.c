/*
 * A352494  Primes congruent to 0, 1, or 10 mod 11.
 * https://oeis.org/A352494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p such that p mod 11 is 0, 1, or 10
 * Range:     u8 n <= 10, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   ensure_capacity, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    static seq_t *primes = NULL;     /* cached list of qualifying primes  */
    static size_t primes_len = 0;    /* number of primes cached            */
    static size_t primes_cap = 0;    /* current allocation capacity        */
    static idx_t next_candidate = 2; /* next integer to test for primality */

    if (n == 0)
        return 0; /* offset 1 – undefined for n 0 */

    /* generate the list until we have at least n terms */
    while (primes_len < n)
    {
        /* find the next prime */
        for (;; ++next_candidate)
        {
            if (is_prime_big(next_candidate))
                break;
        }

        /* check the residue class and collect when wanted */
        if ((next_candidate % (idx_t)11 == 0) ||
            (next_candidate % (idx_t)11 == 1) ||
            (next_candidate % (idx_t)11 == 10))
        {
            /* grow storage if needed */
            ensure_capacity(primes_len + 1,
                            &primes_cap,
                            (void **)&primes,
                            sizeof(seq_t));

            primes[primes_len++] = (seq_t)next_candidate;
        }

        /* advance to the next candidate */
        ++next_candidate;
    }

    return primes[n - 1];
}
