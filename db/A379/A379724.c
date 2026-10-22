/*
 * A379724  a(n) = A000668(n) - 1.
 * https://oeis.org/A379724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^p - 2, where p is the n‑th exponent of a Mersenne prime
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/pow2.h"

/* Known exponents of Mersenne primes (partial list up to 127).  The
 * sequence is defined for these indices; for larger n the function
 * returns 0.  The supplied integer type seq_t is big‑int capable. */
static const idx_t _mersenne_exps[] =
{ /* index (offset 1) -> exponent p */
    2ULL, 3ULL, 5ULL, 7ULL, 13ULL, 17ULL, 19ULL, 31ULL, 61ULL, 89ULL,
   107ULL, 127ULL
};

seq_t a(idx_t n)
{
    /* The sequence offset is 1, so n == 1 corresponds to the first
     * exponent.  If n is out of known range we return 0. */
    if (n == 0)  return 0;

    size_t len = sizeof(_mersenne_exps) / sizeof(_mersenne_exps[0]);

    if (n > len)  return 0;     /* undefined for larger indices */

    /* Compute 2^p - 2 */
    seq_t val = pow2(_mersenne_exps[n-1]);
    val -= 2;
    return val;
}
