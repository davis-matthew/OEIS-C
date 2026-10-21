/*
 * A284633  Numbers n with digits 3 and 6 only.
 * https://oeis.org/A284633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 3 * A007931(n) = number with only digits '3' and '6'
 *            (1‑indexed)
 * Range:     u8 n <= 6, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle offset: values for n < 1 are undefined per specification. */
    if (n == 0) return 0;

    idx_t remaining = n;
    idx_t d = 0;                /* digit length */

    /* Find the length d such that the n‑th term lies among the numbers
       with d digits.  There are 2^d terms of length d. */
    while (1) {
        ++d;
        idx_t count = ((idx_t)1 << d);
        if (remaining > count) {
            remaining -= count;
        } else {
            break;
        }
    }

    /* Zero‑based index within the d‑digit block. */
    idx_t idx = remaining - 1;

    seq_t value = 0;
    for (idx_t pos = 0; pos < d; ++pos) {
        value *= 10;                           /* shift decimal place */
        unsigned int bit = (idx >> (d - 1 - pos)) & 1;
        unsigned int digit = 3 + bit * 3;      /* 3 if bit=0, 6 if bit=1 */
        value += digit;
    }

    return value;
}
