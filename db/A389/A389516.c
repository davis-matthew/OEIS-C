/*
 * A389516  Standard composition numbers of compositions whose maximal runs R
 *          all satisfy length(R) < first(R).
 * https://oeis.org/A389516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer k ≥ 0 such that the composition derived
 *            from k (in standard order A066099) satisfies length(R) <
 *            first(R) for every maximal run R. The implementation enumerates
 *            k until n qualifying numbers are found and caches the results
 *            for repeated calls
 * Range:     u8 n <= 27, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool qualifies(idx_t k)
{
    /* Build the composition corresponding to k. */
    uint64_t bits  = (uint64_t)k;   /* copy for manipulation */
    uint32_t pos_prev = 0;
    uint32_t parts[64];
    uint32_t m = 0, i = 0;          /* m = number of parts */

    while (bits)
    {
        if ((bits & 1ULL) != 0)
        {
            uint32_t pos = i + 1;           /* position in reversed binary */
            parts[m++] = pos - pos_prev;    /* part size */
            pos_prev     = pos;
        }
        ++i;
        bits >>= 1;
    }

    /* Reverse the parts vector. */
    for (uint32_t j = 0; j < m / 2; ++j)
    {
        uint32_t tmp = parts[j];
        parts[j] = parts[m - 1 - j];
        parts[m - 1 - j] = tmp;
    }

    /* Empty composition is acceptable. */
    if (m == 0)
        return true;

    /* Check the maximal runs condition. */
    uint32_t cur = parts[0];
    uint32_t run_len = 1;
    for (uint32_t idx = 1; idx < m; ++idx)
    {
        if (parts[idx] == cur)
            ++run_len;
        else
        {
            if (!(run_len < cur))
                return false;
            cur = parts[idx];
            run_len = 1;
        }
    }
    return run_len < cur;
}

seq_t a(idx_t n)
{
    /* Cache of the first n qualifying indices. */
    static seq_t *good = NULL;
    static size_t good_len = 0;      /* how many cached so far */
    static size_t good_cap = 0;      /* allocated capacity */
    static idx_t next_k = 0;         /* next integer to test */

    /* Generate until we have at least n elements. */
    while (good_len < n)
    {
        if (qualifies(next_k))
        {
            if (good_len == good_cap)
            {
                size_t new_cap = good_cap == 0 ? 128 : good_cap * 2;
                seq_t *tmp = (seq_t *)realloc(good, new_cap * sizeof(seq_t));
                good = tmp;
                good_cap = new_cap;
            }
            good[good_len++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return good[n - 1];
}
