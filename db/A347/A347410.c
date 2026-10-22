/*
 * A347410  Numbers with property that when divided by the sum of their digits
 *          the quotient and remainder are the same.
 * https://oeis.org/A347410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   a(n) is the nth positive integer x such that floor(x /
 *            digit_sum(x)) == x % digit_sum(x), i.e., the quotient and
 *            remainder of x divided by its digit sum are equal
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;      /* sequence is defined only for n >= 1 */

    /* static storage for the sequence values */
    static seq_t *data = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    static seq_t next_x = 2;  /* next integer to test */

    /* generate terms until we have at least n of them */
    while (len < (size_t)n)
    {
        seq_t s = digit_sum(next_x);   /* digit sum of next_x */
        seq_t q = next_x / s;          /* quotient */
        seq_t r = next_x - s * q;      /* remainder */

        if (q == r)                    /* property satisfied */
        {
            if (len + 1 > cap)
            {
                size_t new_cap = cap ? cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(data, new_cap * sizeof(seq_t));
                data = tmp;
                cap  = new_cap;
            }
            data[len++] = next_x;
        }
        ++next_x;
    }

    return data[n - 1];
}
