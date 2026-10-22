/*
 * A321148  a(1) = 24603, a(n) = n*a(n-1) but products that are not in A010784
 *          are first reduced as in A320486 (see comments); continue until
 *          zero is reached.
 * https://oeis.org/A321148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A321148:  a(1) = 24603
   a(n) = reduce( n * a(n-1) )   for n > 1
   The reduce operation keeps only the decimal digits that occur
   exactly once, preserves their order, skips leading zeros,
   and returns 0 if no digits remain.  It is implemented by
   obtaining the digits with get_digits, counting frequencies,
   then reconstructing the number from the most significant
   digit to the least. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/get_digits.h"

/* Reduce a decimal number to the digits that occur only once.
   Digits are processed from most significant to least. */
static seq_t
reduce_value(seq_t x)
{
    unsigned int freq[10] = {0};
    /* A buffer large enough for the decimal digits of the product.
       1024 digits vastly exceed the size needed for all
       intermediate values in the tested ranges. */
    uint8_t buf[1024];
    size_t len = 0;

    get_digits(x, buf, &len);

    /* Count occurrences of each digit. */
    for (size_t i = 0; i < len; ++i)
        freq[buf[i]]++;

    seq_t  res   = 0;
    bool   used  = false;   /* first kept digit has been placed */

    /* Process digits from most significant to least. */
    for (size_t idx = 0; idx < len; ++idx) {
        size_t i = len - 1 - idx;          /* reverse index */
        uint8_t d = buf[i];
        if (freq[d] == 1) {
            if (!used) {                  /* first kept digit */
                if (d == 0)
                    continue;             /* skip leading zero */
                used = true;
                res  = d;
            } else {
                res = res * 10 + d;
            }
        }
    }
    return used ? res : 0;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0ULL;          /* undefined for n < 1, guard anyway */

    seq_t cur = (seq_t)24603;   /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t prod = cur * (seq_t)i;
        cur       = reduce_value(prod);
    }
    return cur;
}
