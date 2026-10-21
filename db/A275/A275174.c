/*
 * A275174  a(n) = (a(n-4) + a(n-1) * a(n-7)) / a(n-8), a(0) = a(1) = ... =
 *          a(7) = 1.
 * https://oeis.org/A275174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 32, u32 n <= 57, u64 n <= 107,
 *            u128 n <= 208, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A275174
   Formula: a(n) = (a(n-4) + a(n-1) * a(n-7)) / a(n-8),   a(0)=a(1)=...=a(7)=1
   For n ≥ 21, the sequence satisfies the linear recurrence
   a(n) = 23*a(n-7) - 23*a(n-14) + a(n-21).
   The implementation uses the original recurrence for 8 ≤ n ≤ 20
   and the linear recurrence thereafter, avoiding large intermediate
   products.  The function is memoised in a static array so that
   calls with arbitrary order of indices are efficient.
*/

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *buf = NULL;
    static size_t capacity = 0;
    static size_t length = 0;   /* number of values already computed */

    /* Guard against an absurdly large n that cannot fit in size_t */
    if (n > SIZE_MAX - 1) {
        /* return 0 as a sensible failure value; the caller may check */
        return (seq_t)0;
    }

    /* Ensure the buffer holds indices up to n */
    if (capacity <= (size_t)n) {
        size_t new_cap = capacity ? capacity * 2 : 16;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;

        seq_t *tmp = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – return zero (caller may ignore) */
            return (seq_t)0;
        }
        buf = tmp;
        capacity = new_cap;
    }

    /* Compute missing values up to n */
    for (size_t i = length; i <= (size_t)n; ++i) {
        if (i <= 7) {
            buf[i] = (seq_t)1;
        } else if (i <= 20) {
            /*  Original recurrence – intermediates stay well within limits
               for i ≤ 20, so 64‑bit arithmetic is safe. */
            buf[i] = (buf[i-4] + buf[i-1] * buf[i-7]) / buf[i-8];
        } else {
            /* Linear recurrence – no multiplication of large numbers */
            buf[i] = (seq_t)(23 * buf[i-7] - 23 * buf[i-14] + buf[i-21]);
        }
    }

    length = (size_t)n + 1;
    return buf[n];
}
