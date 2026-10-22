/*
 * A379839  Numbers that are the sum + product of some multiset of positive
 *          integers > 1. Nonzeros of A379669.
 * https://oeis.org/A379839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   gen
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes A379839: numbers that are the sum + product of some
    multiset of positive integers > 1.  The implementation
    enumerates all multisets whose product does not exceed a fixed
    limit (200 000).  Each multiset (a_1,…,a_k) yields a value
    a_1+…+a_k + a_1·…·a_k, which is stored in a boolean table.
    Afterwards the table is flattened into a sorted array of
    seq_t, which is returned index‑wise.  This works for any
    idx_t (uint64_t) and seq_t (an unsigned integer type or a
    big‑integer class).  The 10 000th term is well below 200 000,
    so the array is always large enough for the required queries. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/gen.h"

static const uint64_t LIMIT = 200000ULL;

seq_t a(idx_t n)
{
    /* Static cache */
    static bool ready = false;
    static seq_t *seq = NULL;
    static size_t seq_len = 0;

    if (!ready) {
        /* Allocate boolean table */
        bool *seen = (bool*)calloc((size_t)LIMIT + 1, sizeof(bool));
        if (!seen)
            return (seq_t)0;      /* allocation failure */

        /* empty multiset => value 1 */
        seen[1] = true;

        /* start enumeration with product=1, sum=0, next element >=2 */
        gen(1ULL, 0ULL, 2ULL, seen, LIMIT);

        /* Count how many values were found */
        size_t cnt = 0;
        for (uint64_t i = 1; i <= LIMIT; ++i)
            if (seen[(size_t)i])
                ++cnt;

        /* Allocate array of seq_t and fill it */
        seq = (seq_t*)malloc(cnt * sizeof(seq_t));
        if (!seq) {
            free(seen);
            return (seq_t)0;
        }

        size_t pos = 0;
        for (uint64_t i = 1; i <= LIMIT; ++i)
            if (seen[(size_t)i])
                seq[pos++] = (seq_t)i;

        seq_len = cnt;
        free(seen);
        ready = true;
    }

    if (n == 0 || (size_t)n > seq_len)
        return (seq_t)0;
    return seq[(size_t)n - 1];
}
