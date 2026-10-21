/*
 * A276004  a(n) is the number of nonzero digits in the factorial-base
 *          representation of n that are matched by more significant digits
 *          from left; a(n) = A060502(n) - A060128(n).
 * https://oeis.org/A276004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9936, u16 n <= 9936, u32 n <= 9936, u64 n <= 9936,
 *            u128 n <= 9936, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A276004:  a(n) = number of nonzero factorial‑base digits that are
   matched by a more significant nonzero digit as defined in the OEIS
   sequence.  Implementation follows the direct definition described
   in the entry. */

#include <stddef.h>

seq_t a(idx_t n)
{
    unsigned char digits[64];
    size_t k = 0;                       /* number of digits found */
    idx_t temp = n;

    /* Convert n to factorial base.
       digits[0] is the least‑significant digit (position 1). */
    while (temp != 0) {
        idx_t base = (idx_t)(k + 2);    /* base for position k+1 is k+2 */
        digits[k] = (unsigned char)(temp % base);
        temp = temp / base;
        ++k;
    }

    seq_t count = 0;

    /* For each nonzero digit, look for a more significant digit that
       satisfies j − d_j = i   (positions are 1‑indexed). */
    for (size_t i = 0; i < k; ++i) {
        if (digits[i] == 0)
            continue;

        for (size_t j = i + 1; j < k; ++j) {
            if (digits[j] == 0)
                continue;

            seq_t diff = (seq_t)(j + 1) - (seq_t)digits[j];
            if (diff == (seq_t)(i + 1)) {
                ++count;
                break;   /* count only one match per i */
            }
        }
    }

    return count;
}
