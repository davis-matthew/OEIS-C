/*
 * A361670  Squarefree part of the n-th triangular number.
 * https://oeis.org/A361670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = squarefree part of n*(n+1)/2
 * Range:     u8 n <= 10, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   build_primes_upto, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/floor_sqrt.h"
#include "../utils/build_primes_upto.h"

/* compute the squarefree part of a positive integer m.
   The function removes all square factors from m. */
static seq_t
squarefree_part_generic(seq_t m)
{
    seq_t result = 1;

    /* limit for prime search */
    seq_t sqrt_m  = floor_sqrt(m);
    idx_t  limit  = (idx_t)sqrt_m;          /* safe cast: sqrt(m) fits in idx_t */

    /* prepare prime list up to 'limit' */
    size_t prime_cnt = 0;
    uint64_t *primes = NULL;
    build_primes_upto(limit, &primes, &prime_cnt);

    /* factor out primes up to sqrt(m) */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t  p     = primes[i];
        seq_t     pseq  = (seq_t)p;
        int       parity = 0;          /* 0 even, 1 odd exponent */

        while (m % pseq == 0) {
            m /= pseq;
            parity ^= 1;
        }
        if (parity) {
            result *= pseq;           /* include prime only if exponent odd */
        }
    }

    /* remaining factor (if any) is prime and has exponent 1 */
    if (m > 1) {
        result *= m;
    }

    return result;
}

/* OEIS sequence A361670: squarefree part of the nth triangular number */
seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n * ((seq_t)n + 1) / 2;
    return squarefree_part_generic(tn);
}
