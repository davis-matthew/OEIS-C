/*
 * A346508  Positive integers k such that 10*k+1 is equal to the product of
 *          two integers greater than 1 and ending with 1 (A346507).
 * https://oeis.org/A346508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th integer k such that 10*k+1 is a product of two
 *            integers >1 ending with 1. The function searches for the
 *            required k
 * Range:     u8 none, u16 n <= 55, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* Test whether k satisfies the property: 10*k+1 = p*q with p,q>1 and p%10==q%10==1. */
static bool qualifies(seq_t k)
{
    seq_t N = (seq_t)10 * k + 1;
    seq_t sqrtN = floor_sqrt(N);          /* integer sqrt of N */
    seq_t d;
    for (d = (seq_t)11; d <= sqrtN; d = d + 10) {
        if (N % d == 0) {
            seq_t e = N / d;
            if (e % (seq_t)10 == 1)
                return true;
        }
    }
    return false;
}

/* Return the n-th term of the sequence (with a(1) being the first term). */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n=0; return 0 for safety */
        return 0;

    idx_t count = 0;
    idx_t k = 0;          /* candidate integer k (as idx_t) */
    while (count < n) {
        ++k;
        if (qualifies((seq_t)k))
            ++count;
    }
    return (seq_t)k;
}
