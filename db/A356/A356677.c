/*
 * A356677  The lexicographically earliest infinite squarefree sequence of
 *          nonnegative integers that starts with 1.
 * https://oeis.org/A356677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   lexicographically earliest infinite squarefree sequence over
 *            nonnegative integers that starts with 1. Constructed greedily:
 *            at each step pick the smallest integer that does not create a
 *            square ending at the new position
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/ensure_capacity.h"   // provides: void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size);

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *seq = NULL;
    static idx_t cap = 0;   /* capacity in elements */
    static idx_t cur = 0;   /* number of elements produced so far */

    if (n == 0)   /* undefined offset; return 0 */
        return 0;

    /* enlarge cache if necessary */
    if (cap < n)
        ensure_capacity((size_t)n, &cap, (void **)&seq, sizeof(seq_t));

    /* generate terms until we reach the requested n */
    while (cur < n)
    {
        if (cur == 0)  /* first term is fixed to 1 */
        {
            seq[0] = (seq_t)1;
            cur = 1;
            continue;
        }

        /* find smallest candidate that keeps the sequence squarefree */
        seq_t cand = 0;
        for (;; ++cand)
        {
            bool ok = true;
            idx_t L = cur + 1;              /* length after adding cand */

            /* check all possible square lengths that end at the new element */
            for (idx_t len = 1; len <= L / 2; ++len)
            {
                bool equal = true;
                for (idx_t i = 0; i < len; ++i)
                {
                    /* compare the i-th element of the last block with the
                       corresponding element of the previous block
                       (the last element of the new block is cand). */
                    seq_t v1 = seq[cur - 1 - i];
                    seq_t v2 = (i == len - 1) ? cand : seq[cur - 1 - i - len];
                    if (v1 != v2)
                    {
                        equal = false;
                        break;
                    }
                }
                if (equal)
                {
                    ok = false;
                    break;
                }
            }

            if (ok)   /* found a valid candidate */
            {
                seq[cur] = cand;
                ++cur;
                break;
            }
        }
    }

    return seq[n - 1];
}
