/*
 * A293875  Numbers having '15' as substring of their digits.
 * https://oeis.org/A293875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 2989, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence A293875: the nth positive integer whose decimal
   representation contains the substring "15".  */

#include <stdbool.h>
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

/* Return true if number x contains the two consecutive digits '1' and '5'. */
static bool has15(seq_t x)
{
    if (x < (seq_t)15)          /* No way to contain "15" */
        return false;

    idx_t len = digit_len(x);
    if (len < 2)                /* Not enough digits */
        return false;

    seq_t power = pow10(len - 1);          /* Highest power of 10 */
    seq_t prev   = (x / power) % (seq_t)10;/* Most significant digit */

    for (idx_t i = 1; i < len; ++i)
    {
        power /= (seq_t)10;              /* Move to next lower digit place */
        seq_t cur = (x / power) % (seq_t)10;
        if (prev == 1 && cur == 5)
            return true;
        prev = cur;
    }
    return false;
}

/* OEIS A293875: a(n) is the nth integer containing "15" as a substring. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t val   = 0;

    while (count < n)
    {
        ++val;
        if (has15(val))
            ++count;
    }
    return val;
}
