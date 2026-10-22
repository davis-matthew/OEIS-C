/*
 * A373953  Sum of run-compression of the n-th integer composition in standard
 *          order.
 * https://oeis.org/A373953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A373953: sum of run‑compression of the n-th integer composition
 * in standard (graded reverse‑lexicographic) order. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* find smallest m with 2^m > n  (i.e., m = floor_log2(n) + 1) */
    idx_t bitpos = 63 - __builtin_clzll((unsigned long long)n);
    idx_t m = bitpos + 1;

    /* offset of n within compositions of sum m */
    idx_t offset = n - ((idx_t)1 << (m - 1));

    seq_t sum   = 0;
    seq_t part  = 0;
    seq_t prev  = (seq_t)-1;   /* sentinel different from any valid part */

    /* iterate over the m unit elements of the composition */
    for (idx_t unit = 1; unit <= m; ++unit)
    {
        ++part;  /* add the current unit to the running part */

        /* decide whether to cut after this unit (except after last unit) */
        if (unit < m)
        {
            idx_t pos      = unit;                     /* cut after this unit */
            idx_t bitIndex = (m - 1) - pos;             /* position in offset bits */
            if (offset & ((idx_t)1 << bitIndex))
            {
                if (part != prev)
                    sum += part;   /* add first part of a new run */
                prev = part;
                part = 0;         /* start next part */
            }
        }
    }

    /* finalize the last part */
    if (part > 0 && part != prev)
        sum += part;

    return sum;
}
