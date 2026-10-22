/*
 * A378119  a(n) is the smallest positive k such that the digit sums of k and
 *          k + 1 are both divisible by n, or -1 if no such pair exists.
 * https://oeis.org/A378119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = smallest k such that the digit sums of k and k + 1 are
 *            both * divisible by n, or -1 if no such k exists. For n multiple
 *            of 3 the sequence * is always -1. For other n: * a(n) =
 *            A051885(n) * 10^q - 1 * where A051885(n) is the smallest number
 *            whose digit sum equals n, * and q is the modular i...
 * Range:     i8 n <= 4, i16 n <= 4, i32 n <= 9, i64 n <= 18, i128 n <= 30,
 *            big any n
 * Helpers:   modinv9, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"
#include "../utils/modinv9.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;
    if ((n % (idx_t)3) == 0)
        return (seq_t)-1;

    /* Compute A051885(n): smallest number with digit sum n. */
    idx_t r = n % (idx_t)9;
    idx_t k = n / (idx_t)9;
    seq_t val = 0;
    if (r > 0)
        val = (seq_t)r;
    for (idx_t i = 0; i < k; ++i)
    {
        val = val * (seq_t)10 + (seq_t)9;
    }

    /* Compute q = 9^(-1) mod n. */
    idx_t q = modinv9(n);

    /* Compute 10^q and combine. */
    seq_t powq = pow10(q);
    seq_t result = val * powq;
    result -= (seq_t)1;
    return result;
}
