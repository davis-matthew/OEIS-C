/* is_square: whether n is a perfect square.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IS_SQUARE_H
#define OEISC_UTILS_IS_SQUARE_H
#include <stdbool.h>
#include <stdint.h>

/* Determine if x is a perfect square.  Works for any seq_t that supports
   + - * / comparisons and ++/--.  The routine uses binary search on [0,x]. */
static bool is_square(seq_t x)
{
    if (x < 0)
        return false;
    seq_t low = (seq_t)0;
    seq_t high = x;
    while (low < high) {
        seq_t mid = (low + high + (seq_t)1) >> 1;  /* (low+high+1)/2 */
        seq_t sq = mid * mid;
        if (sq <= x)
            low = mid;
        else
            high = mid - (seq_t)1;
    }
    return low * low == x;
}

#endif
