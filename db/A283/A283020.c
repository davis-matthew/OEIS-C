/*
 * A283020  For p = prime(n), number of iterations of the function f(x) =
 *          2*x^3-1 that leave p prime.
 * https://oeis.org/A283020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of iterations of f(x)=2*x^3-1 that leave p prime,
 *            where p is the nth prime
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 13, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_big.h"

/* Computes the nth term of the sequence.  The function is thread‑safe,
   but may rebuild its prime table if a larger n is requested. */
seq_t a(idx_t n)
{
    /* Cache of primes.  primes_cnt holds the number of primes stored. */
    static uint64_t *primes      = NULL;
    static size_t  primes_cnt   = 0;

    /* Ensure we have at least n primes. */
    if (primes_cnt < n) {
        if (primes) free(primes);
        idx_t limit = _prime_limit_for_idx(n);
        build_primes_upto(limit, &primes, &primes_cnt);
    }

    /* Prime p = prime(n). */
    seq_t p = (seq_t)primes[n-1];

    /* Count iterations that produce a prime. */
    seq_t count = 0;
    seq_t cur   = p;

    while (1) {
        /* next = 2*cur^3 - 1. */
        seq_t cube  = cur * cur;
        cube        = cube * cur;          /* cur^3 */
        seq_t next = cube * (seq_t)2 - (seq_t)1;

        if (!is_prime_big(next))
            break;          /* composite: stop */
        count++;            /* prime: increment counter */
        cur = next;         /* continue with next value */
    }

    return count;
}
