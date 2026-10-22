/*
 * A343695  a(n) is the number of preference profiles in the stable marriage
 *          problem with n men and n women, where men prefer different women
 *          and women prefer different men as their first choices.
 * https://oeis.org/A343695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n!)^2 * ((n-1)!)^(2*n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   big_pow, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                 /* for size_t */
#include "../utils/factorial.h"     /* seq_t factorial(idx_t n) */
#include "../utils/big_pow.h"       /* seq_t big_pow(seq_t base, seq_t exp) */

seq_t a(idx_t n) {
    /* compute factorials */
    seq_t fac_n  = factorial(n);
    seq_t fac_nm1 = factorial(n-1);

    /* compute the large power ((n-1)!)^(2*n) */
    seq_t pow_part = big_pow(fac_nm1, (seq_t)(2*n));

    /* final result: (n!)^2 * pow_part */
    seq_t result = fac_n * fac_n;
    result = result * pow_part;

    return result;
}
