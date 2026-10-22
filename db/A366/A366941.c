/*
 * A366941  a(n) is the least k such that k == 1 (mod 6) and k has exactly n
 *          prime factors, counted with multiplicity.
 * https://oeis.org/A366941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n if n is even, 5^(n-1)*7 if n is odd
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if ((n & 1u) == 0u)            /* n is even */
        return big_pow((seq_t)5, (seq_t)n);
    else                           /* n is odd */
        return big_pow((seq_t)5, (seq_t)(n - 1)) * (seq_t)7;
}
