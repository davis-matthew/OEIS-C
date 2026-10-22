/*
 * A329192  Fibonacci numbers with arithmetic mean of digits an integer (sum
 *          of digits = a multiple of number of digits).
 * https://oeis.org/A329192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Fibonacci number whose digits arithmetic mean is an
 *            integer
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 12, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   digit_len, digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: generate Fibonacci numbers one by one and keep the ones
 * that satisfy sum_of_digits % number_of_digits == 0.  The first such
 * number is F(1)=1.  The next is F(3)=2, and so forth. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/digit_sum.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    /* n is assumed to be ≥ 1. */
    if (n == (idx_t)1)
        return (seq_t)1;          /* F(1) */

    /* Current Fibonacci numbers: a_prev = F(i-1), a_curr = F(i) */
    seq_t a_prev = (seq_t)1;      /* F(1) */
    seq_t a_curr = (seq_t)1;      /* F(2) */

    idx_t count = 1;              /* F(1) has already been counted */

    while (count < n)
    {
        /* Next Fibonacci number: F(i+1) = F(i-1) + F(i) */
        seq_t next = a_prev + a_curr;
        a_prev   = a_curr;
        a_curr   = next;

        /* Test the integer-mean condition */
        unsigned long long sum = (unsigned long long)digit_sum(next);
        size_t          len = digit_len(next);
        if (len == 0)
            len = 1;              /* safety: empty number cannot occur */

        if (sum % len == 0)
        {
            ++count;
            if (count == n)
                return next;     /* Found the n-th term */
        }
    }

    /* The function is guaranteed to return before reaching this point. */
    return (seq_t)0;
}
