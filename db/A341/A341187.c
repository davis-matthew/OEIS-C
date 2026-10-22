/*
 * A341187  Numbers that when divided by the sum of their digits leave 17 as
 *          remainder.
 * https://oeis.org/A341187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1316, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers n such that n mod digit_sum(n) == 17 */
#include <stdlib.h>

/* Compute the sum of the decimal digits of v. */
static seq_t sum_digits(seq_t v)
{
    seq_t s = 0;
    while (v > (seq_t)0) {
        s += v % (seq_t)10;
        v /= (seq_t)10;
    }
    return s;
}

/* Cache of found terms.  The cache is grown lazily. */
static seq_t *cache   = NULL;
static size_t  cap    = 0;
static size_t  cnt    = 0;
static seq_t  nextNum = 1;

/* Return the n-th term (a(1) is the first term). */
seq_t a(idx_t n)
{
    /* Offset is 1, but return 0 for n==0 just in case. */
    if (n == (idx_t)0) return (seq_t)0;

    /* Keep generating terms until we have at least n of them. */
    while (cnt < (size_t)n) {
        seq_t d = sum_digits(nextNum);
        /* digit sum must be at least 18 because 17 is the remainder. */
        if (d >= (seq_t)18 && (nextNum % d) == (seq_t)17) {
            /* Grow cache if necessary. */
            if (cnt == cap) {
                size_t newCap = cap == 0 ? 128 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, newCap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cap = newCap;
            }
            cache[cnt++] = nextNum;
        }
        nextNum++;
    }

    return cache[(size_t)(n - (idx_t)1)];
}
