/*
 * A353518  a(n) = 1 if n is a product of superprimorials (A006939), otherwise
 *          0.
 * https://oeis.org/A353518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is a product of superprimorials (A006939),
 *            otherwise 0. A superprimorial is a number whose prime
 *            factorization contains only 2 and 3, with exponent of 2 strictly
 *            greater than exponent of 3, and that exponent of 3 (if present)
 *            is odd. The empty product (n = 1) counts as 1
 * Range:     u8 n <= 9599, u16 n <= 9599, u32 n <= 9599, u64 n <= 9599,
 *            u128 n <= 9599, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* the empty product n = 1 gives a(1) = 1 */
    if (n == 1)
        return (seq_t)1;

    /* n fits in uint64_t because idx_t is uint64_t */
    uint64_t val = (uint64_t)n;
    uint64_t e2 = 0, e3 = 0;

    /* factor out powers of 2 */
    while ((val & 1ULL) == 0ULL)
    {
        ++e2;
        val >>= 1;
    }

    /* factor out powers of 3 */
    while (val % 3ULL == 0ULL)
    {
        ++e3;
        val /= 3ULL;
    }

    /* any remaining prime factor invalidates the product */
    if (val != 1ULL)
        return (seq_t)0;

    /* need exponent of 2 strictly greater than exponent of 3 */
    if (e2 <= e3)
        return (seq_t)0;

    /* exponent of 3 must be zero or odd */
    if (e3 != 0 && (e3 & 1ULL) == 0ULL)
        return (seq_t)0;

    return (seq_t)1;
}
