/*
 * A330819  Numbers of the form M_p^2(M^p+2)^2, where M_p is a Mersenne prime
 *          (A000668) and p is a Mersenne exponent (A000043). Also the first
 *          element of the spectral basis of A330817.
 * https://oeis.org/A330819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = M_p^2*(M_p+2)^2, where M_p = 2^p-1 and p is the n-th *
 *            Mersenne exponent (A000043)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   pow2, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/seq_mul.h"

/* List of the first 40 Mersenne exponents (A000043). */
static const idx_t mersenne_exponents[] = {
    2ULL, 3ULL, 5ULL, 7ULL, 13ULL, 17ULL, 19ULL, 31ULL, 61ULL, 89ULL,
    107ULL, 127ULL, 521ULL, 607ULL, 1279ULL, 2203ULL, 2281ULL, 3217ULL,
    4253ULL, 4423ULL, 9689ULL, 9941ULL, 11213ULL, 19937ULL, 21701ULL,
    23209ULL, 44497ULL, 86243ULL, 110503ULL, 132049ULL, 216091ULL,
    756839ULL, 859433ULL, 1257787ULL, 1398269ULL, 2976221ULL, 3021377ULL,
    3449671ULL, 5226163ULL, 12478027ULL
};

static const size_t num_exponents =
    sizeof(mersenne_exponents) / sizeof(mersenne_exponents[0]);

seq_t a(idx_t n)
{
    /* Sequence is defined for n >= 1.  Return 0 for out-of-range. */
    if (n == 0) return 0;

    /* Convert idx_t to size_t for bounds checking. */
    if ((size_t)(n - 1) >= num_exponents) {
        /* Not enough known Mersenne primes.  The problem guarantees that
         * n will not exceed the number of available terms.  Return 0
         * or an indicative value. */
        return 0;
    }

    /* p is the n-th Mersenne exponent. */
    idx_t p = mersenne_exponents[n - 1];

    /* Compute two = 2^p as seq_t. */
    seq_t two = pow2(p);

    /* M_p = two - 1. */
    seq_t M = two - 1;

    /* term = M * (M + 2) = M * (two + 1). */
    seq_t term = seq_mul(M, two + 1);

    /* a(n) = term^2. */
    return seq_mul(term, term);
}
