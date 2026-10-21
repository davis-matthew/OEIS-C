/*
 * A286607  Numbers that are not divisible by the sum of their factorial base
 *          digits (A034968).
 * https://oeis.org/A286607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 192, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A286607: Numbers that are not divisible by the sum of their factorial
 * base digits (A034968). */
#include <stddef.h>

/* Compute sum of digits of x in factorial base. */
static seq_t sum_fact_digits(seq_t x)
{
    seq_t sum = 0;
    idx_t k = 1;                      /* position index (start at 1 for 1!) */
    while (x != 0) {
        seq_t div   = (seq_t)(k + 1);  /* (k+1)! / k! = k+1 */
        seq_t digit = x % div;
        sum += digit;
        x /= div;
        ++k;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0)                /* offset is 1; safety check */
        return 0;

    idx_t found = 0;
    idx_t cand  = 1;            /* candidate integer to test */

    while (1) {
        seq_t val = (seq_t)cand;
        seq_t s   = sum_fact_digits(val);
        if (s != 0 && (val % s) != 0) {
            ++found;
            if (found == n)    /* found the n-th term */
                return val;
        }
        ++cand;
    }
}
