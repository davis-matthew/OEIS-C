/*
 * A260862  Base-12 representation of a(n) is the concatenation of the base-12
 *          representations of 1, 2, ..., n, n-1, ..., 1.
 * https://oeis.org/A260862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenation of base-12 representations of 1,2,…,n,
 *            n-1,…,1 (value interpreted in base 10)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   digits_base12, pow12_of_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/digits_base12.h"
#include "../utils/pow12_of_digits.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t res = 0;
    /* forward part: 1,2,…,n */
    for (idx_t i = 1; i <= n; ++i)
    {
        int d = digits_base12(i);
        res = res * pow12_of_digits(d) + (seq_t)i;
    }
    /* backward part: n-1,…,1 */
    for (idx_t i = n; i > 1; --i)
    {
        idx_t j = i - 1;
        int d = digits_base12(j);
        res = res * pow12_of_digits(d) + (seq_t)j;
    }
    return res;
}
