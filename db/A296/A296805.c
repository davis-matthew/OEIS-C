/*
 * A296805  Sum of the larger parts in the partitions of n into two distinct
 *          parts with the larger part even.
 * https://oeis.org/A296805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor((n-1)/2)} (n-i) * ((n-i+1) mod 2)
 * Range:     u8 n <= 38, u16 n <= 590, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the required sequence for all n >= 1. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n < 3 the sum is empty, return 0. */
    if (n < 3) {
        return 0;
    }

    /* Use the closed‑form summation of even numbers between
       floor(n/2)+1 and n-1.  This is O(1). */

    seq_t start = (seq_t)(n / 2 + 1);          /* first integer > n/2 */
    seq_t end   = (seq_t)(n - 1);              /* largest large part    */

    /* Adjust to the nearest even integers. */
    seq_t first_even = (start & 1) ? start + 1 : start;   /* make even */
    seq_t last_even  = (end & 1)  ? end - 1 : end;        /* make even */

    if (first_even > last_even) {
        return 0;   /* no even large part for this n */
    }

    /* Number of even terms */
    seq_t count = ((last_even - first_even) / 2) + 1;

    /* Sum of an arithmetic series: k·(first + last)/2 */
    /* All operations are integer, truncating as required. */
    seq_t sum = count * (first_even + last_even) / 2;

    return sum;
}
