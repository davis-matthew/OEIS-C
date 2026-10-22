/*
 * A346008  Order of the full automorphism group of an n^2 X n^2 Sudoku
 *          puzzle.
 * https://oeis.org/A346008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * (n!)^(2*n+2) for n > 1; a(1) = 1
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   big_pow, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factorial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    seq_t fac = factorial(n);
    seq_t exp = (seq_t)(2 * n + 2);
    seq_t power = big_pow(fac, exp);
    return 2 * power;
}
