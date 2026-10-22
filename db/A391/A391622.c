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
 * Range:     u8 n <= 59, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   unique_for_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/unique_for_k.h"

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
