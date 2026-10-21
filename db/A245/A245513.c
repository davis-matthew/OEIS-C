/*
 * A245513  Smallest m such that neither of the two odd numbers that bracket
 *          n^m is a prime.
 * https://oeis.org/A245513
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m such that neither of the two odd numbers that
 *            * bracket n^m is a prime. * For even n the pair is (n^m-1,
 *            n^m+1); for odd n it is * (n^m-2, n^m+2)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;   /* offset starts at 2; values below are unused */

    const bool even = (n & 1) == 0;

    for (idx_t m = 1; ; ++m)
    {
        seq_t pow = pow_uint((uint64_t)n, m);          /* compute n^m */

        seq_t left, right;
        if (even) {
            left  = pow - 1;
            right = pow + 1;
        } else {
            left  = pow - 2;
            right = pow + 2;
        }

        /* If both odd numbers are composite, m is the answer */
        if (!isPrimeGeneric(left) && !isPrimeGeneric(right))
            return (seq_t)m;
    }
}
