/*
 * A350659  a(n) is the maximal digit, expressed in base 10, in the base-n
 *          representation of 2^n.
 * https://oeis.org/A350659
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = maximal digit of 2^n when written in base n
 * Range:     u8 n <= 259, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: build the base‑n representation of 2^n by repeated
 * multiplication by 2, keeping the digits in an array of idx_t.
 * Afterwards, take the maximum digit.  This works for all
 * idx_t/seq_t builds and satisfies the time/accuracy constraints. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;                 /* sequence is defined from n=2 */

    /* Use an array of idx_t to store the base‑n digits.
       Digits are always < n <= 10 000 → fit into idx_t.      */
    idx_t *digits = NULL;
    size_t cap   = 1;                    /* current capacity */
    size_t len   = 1;                    /* number of used digits */
    digits = (idx_t *)malloc(cap * sizeof(idx_t));
    if (!digits) return 0;               /* allocation failure */

    digits[0] = 1;                       /* start with 1 (2^0) */

    /* Multiply by 2 exactly n times to reach 2^n. */
    for (idx_t step = 0; step < n; ++step)
    {
        idx_t carry = 0;
        for (size_t i = 0; i < len; ++i)
        {
            idx_t temp = digits[i] * 2 + carry;
            digits[i] = temp % n;
            carry     = temp / n;
        }
        while (carry)
        {
            if (len == cap)                         /* grow the array */
            {
                cap *= 2;
                idx_t *tmp = (idx_t *)realloc(digits, cap * sizeof(idx_t));
                if (!tmp) { free(digits); return 0; }
                digits = tmp;
            }
            digits[len++] = carry % n;
            carry        /= n;
        }
    }

    /* Find the maximum digit. */
    idx_t maxd = 0;
    for (size_t i = 0; i < len; ++i)
        if (digits[i] > maxd) maxd = digits[i];

    free(digits);
    return (seq_t)maxd;
}
