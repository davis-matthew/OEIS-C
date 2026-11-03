/*
 * A391622  Numbers k such that there is a unique nonnegative sequence whose
 *          first sums are the k-th composition in standard order.
 * https://oeis.org/A391622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) is the n‑th positive integer k such that * the k‑th
 *            composition (A066099) in standard order has a * unique
 *            non‑negative integer sequence whose first sums equal * that
 *            composition. The algorithm tests each k in * increasing order,
 *            using the property that the solution * exists iff the
 *            intersec...
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

static bool unique_for_k(idx_t k)
{
    /* Build composition parts from the binary representation of k.
     * parts[i] = difference between consecutive 1‑bit positions
     * (with an implicit 0 at the start).  The number of parts
     * equals the number of set bits in k. */
    int64_t parts[65];
    int m = 0;

    idx_t tmp = k;
    int64_t prev_pos = 0;         /* position of the previous 1‑bit   */
    int64_t bit_index = 0;        /* 0‑based index of current bit     */

    while (tmp)
    {
        if (tmp & 1ULL)
        {
            int64_t curr_pos = bit_index + 1;          /* positions start at 1 */
            parts[m++] = curr_pos - prev_pos;
            prev_pos = curr_pos;
        }
        tmp >>= 1;
        ++bit_index;
    }

    /* Determine lower and upper bounds on the first term a0. */
    int64_t B = 0;                 /* B_0 = 0                          */
    int64_t lower = 0;              /* maximal lower bound (starts at 0) */
    int64_t upper = INT64_MAX / 2;  /* minimal upper bound              */

    for (int j = 1; j <= m; ++j)        /* j is 1‑based index of part */
    {
        B = parts[j - 1] - B;

        if (j & 1)          /* odd j → upper bound            */
        {
            if (B < upper) upper = B;
        }
        else                /* even j → lower bound           */
        {
            int64_t bound = -B;
            if (bound > lower) lower = bound;
        }
    }

    /* Unique solution exists iff bounds coincide. */
    return lower == upper;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* offset is 1; n<1 is undefined */

    idx_t count = 0;
    idx_t k = 1;
    seq_t result = 0;

    while (1)
    {
        if (unique_for_k(k))
        {
            ++count;
            if (count == n)
            {
                result = (seq_t)k;
                break;
            }
        }
        ++k;
    }
    return result;
}
