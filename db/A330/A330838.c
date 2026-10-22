/*
 * A330838  Numbers of the form 2^(2*p)*3*M_p^2, where p > 2 is a Mersenne
 *          exponent, A000043, and M_p is the corresponding Mersenne prime,
 *          A000668.
 * https://oeis.org/A330838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 * 2^(2p) * (2^p - 1)^2, where p is the nth Mersenne
 *            exponent > 2
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 4, u128 n <= 7,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Mersenne exponents > 2 (known up to 4423, which yields terms of modest size). */
    static const idx_t p_list[] = {
        3ULL, 5ULL, 7ULL, 13ULL, 17ULL, 19ULL, 31ULL, 61ULL,
        89ULL, 107ULL, 127ULL, 521ULL, 607ULL, 1279ULL,
        2203ULL, 2281ULL, 3217ULL, 4253ULL, 4423ULL
    };
    const size_t len = sizeof(p_list) / sizeof(p_list[0]);

    if (n == 0 || n > len)
        return 0;   /* Out of known range */

    const idx_t p = p_list[n - 1];

    seq_t pow2_2p = pow2(2 * p);          /* 2^(2p)          */
    seq_t Mp      = pow2(p) - 1;          /* 2^p - 1         */
    seq_t term    = 3 * pow2_2p * (Mp * Mp);  /* 3 * 2^(2p) * (2^p-1)^2 */

    return term;
}
