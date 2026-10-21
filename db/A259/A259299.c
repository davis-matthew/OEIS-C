/*
 * A259299  The decimal expansion of n/(n+1) until it terminates or repeats,
 *          shown without the decimal point.
 * https://oeis.org/A259299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the integer formed by the decimal expansion of n/(n+1)
 *            (without the decimal point). The expansion stops when the
 *            remainder becomes zero (terminating decimal) or repeats
 *            (repeating decimal)
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 15, u64 n <= 21, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* The function uses only the operations allowed by the specification
   and works for any seq_t implementation that supports the usual
   integer arithmetic and implicit conversion to unsigned long long
   for values small enough to fit in a size_t (the test range is
   limited to n < 10000).  */
seq_t a(idx_t n)
{
    /* a(0) is defined to be 0.  */
    if (n == 0) {
        return 0;
    }

    /* The denominator is n+1, stored in seq_t for use in the arithmetic.  */
    seq_t denom = (seq_t)n + 1;

    /* For the remainder tracking array we need a size_t that fits the value
       of denom (which is <= 10000 in the test data).  We compute the same
       value using the index type to avoid any conversion issues for
       arbitrary-precision seq_t.  */
    uint64_t denom_ul = (uint64_t)n + 1;
    size_t d = (size_t)denom_ul;

    /* Allocate a bit array to remember which remainders have been seen.  */
    uint8_t *seen = (uint8_t *)calloc(d, 1);
    if (!seen) {
        /* In the unlikely event of allocation failure, return 0 as a
           graceful fallback.  */
        return 0;
    }

    /* The current remainder starts with n.  */
    seq_t rem = (seq_t)n;
    seq_t result = 0;   /* Accumulated decimal digits.  */

    while (1) {
        if (rem == 0) {
            /* Terminating decimal.  */
            break;
        }

        /* The remainder is guaranteed to be less than denom, which fits
           in a size_t for the given test range.  */
        size_t idx = (size_t)( (uint64_t)rem );  /* cast via unsigned long long */

        if (seen[idx]) {
            /* We have seen this remainder before: the decimal repeats.  */
            break;
        }

        seen[idx] = 1;   /* Mark this remainder as seen.  */

        rem *= (seq_t)10;
        seq_t digit = rem / denom;     /* Next decimal digit.  */
        rem = rem % denom;             /* New remainder.  */

        result = result * (seq_t)10 + digit;  /* Append digit.  */
    }

    free(seen);
    return result;
}
