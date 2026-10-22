/*
 * A359818  Dirichlet inverse of A359549, where A359549 is the characteristic
 *          function for numbers that are either an odd squarefree number
 *          squared or twice such a number.
 * https://oeis.org/A359818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   multiplicative; a(1)=1. For each prime power p^e in the
 *            factorisation of n: a(p^e) = (-1)^e if p == 2 a(p^e) = 0 if p >
 *            2 and e is odd a(p^e) = (-1)^(e/2) if p > 2 and e is even The
 *            Dirichlet inverse of A359549 is therefore the product of these
 *            local factors
 * Range:     i8 n <= 8867, i16 n <= 8867, i32 n <= 8867, i64 n <= 8867,
 *            i128 n <= 8867, big any n
 * Helpers:   _smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_smallest_prime_factor.h"

seq_t a(idx_t n)
{
    if (n <= 1)          /* offset: a(1) = 1; n=0 is unused */
        return n == 1 ? 1 : 0;

    seq_t result = 1;
    idx_t x = n;

    while (x > 1) {
        uint64_t p = _smallest_prime_factor((uint64_t)x);
        if (p == 0)          /* n is prime */
            p = x;

        idx_t exp = 0;
        while (x % p == 0) {
            x /= p;
            ++exp;
        }

        seq_t coeff;
        if (p == 2ULL) {           /* a(2^e) = (-1)^e */
            coeff = (exp & 1) ? -1 : 1;
        } else {                   /* p > 2 */
            if (exp & 1)           /* odd exponent => 0 */
                return 0;
            coeff = ((exp >> 1) & 1) ? -1 : 1; /* (-1)^(e/2) */
        }

        result *= coeff;
        if (result == 0)
            return 0;
    }

    return result;
}
