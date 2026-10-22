/*
 * A377842  a(n) = q - 2*p, where q is the greatest prime such that p=2*n - q
 *          is also prime.
 * https://oeis.org/A377842
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = q - 2*p where q is the greatest prime such that p = 2*n -
 *            q is also prime
 * Range:     i8 n <= 69, i16 n <= 71, i32 n <= 71, i64 n <= 71, i128 n <= 71,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t) */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 2.  For any other n we return 0. */
    if (n < 2)
        return (seq_t)0;

    /* The sum of the two primes sought is 2*n. */
    idx_t target = n * (idx_t)2;

    /* Search for the largest prime q <= target with p = target - q also prime. */
    for (idx_t q = target; q >= 2; --q)
    {
        if (!is_prime(q))
            continue;

        idx_t p = target - q;
        if (p < 2)
            continue;
        if (!is_prime(p))
            continue;

        /* Compute a(n) = q - 2*p in seq_t arithmetic. */
        seq_t sq = (seq_t)q;
        seq_t sp = (seq_t)p;
        return sq - (seq_t)2 * sp;
    }

    /* For n >= 2 a suitable pair (p, q) always exists according to the definition,
       so this point should never be reached.  Return 0 as a safety fallback. */
    return (seq_t)0;
}
