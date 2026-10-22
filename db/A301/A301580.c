/*
 * A301580  a(n) is the number of positive integers between successive
 *          Mersenne primes.
 * https://oeis.org/A301580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A301580: a(n) = 2^{p_{n+1}} − 2^{p_n} − 1
   where p_n are the exponents of Mersenne primes.  */

/* This implementation uses the known list of Mersenne‑prime exponents
   up to the 51st Mersenne prime (exponent 82589933).  For any index n
   beyond the number of known gaps it returns 0.  The implementation
   relies on the utility function pow2 defined in "../utils/pow2.h".  */

#include <stddef.h>
#include "../utils/pow2.h"   /* pow2(seq_t exp) returns 2^exp */

static const idx_t mersenne_exponents[] = {
    2U, 3U, 5U, 7U, 13U, 17U, 19U, 31U, 61U, 89U,
    107U, 127U, 521U, 607U, 1279U, 2203U, 2281U,
    3217U, 4253U, 4423U, 9689U, 9941U, 11213U,
    19937U, 21701U, 23209U, 44497U, 86243U,
    110503U, 132049U, 216091U, 756839U, 859433U,
    1257787U, 1398269U, 2976221U, 3021377U,
    6972593U, 13466917U, 20996011U, 24036583U,
    25964951U, 30402457U, 32582657U, 37156667U,
    42643801U, 43112609U, 57885161U, 74214481U,
    77232917U, 82589933U
};

seq_t a(idx_t n)
{
    /* a(1) corresponds to the gap between the first two Mersenne primes,
       i.e. between exponents 2 and 3.  The array contains len exponents,
       so the maximum valid n is len-1.  */
    const size_t len = sizeof mersenne_exponents / sizeof mersenne_exponents[0];
    if (n == 0 || n >= len)
        return (seq_t)0;

    /* Compute 2^{p_{n+1}} and 2^{p_n} using the utility `pow2`. */
    seq_t high  = pow2(mersenne_exponents[n]);     /* p_{n+1}  */
    seq_t low   = pow2(mersenne_exponents[n - 1]); /* p_n      */

    return high - low - (seq_t)1;
}
