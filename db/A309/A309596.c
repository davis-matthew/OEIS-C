/*
 * A309596  If a(n) is not a term of a(0..n-1), then a(n+1) = a(n) - a(n -
 *          a(n)); otherwise a(n+1) is the number of terms equal to a(n) in
 *          a(0..n-1). Start with a(0)=0, a(1)=0.
 * https://oeis.org/A309596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(0)=0, a(1)=0. For n>=1, if a(n) has not appeared in a(0..n-1)
 *            then a(n+1)=a(n)-a(n-a(n)); otherwise a(n+1)= number of
 *            occurrences of a(n) in a(0..n-1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static memoization of the sequence up to the highest index requested. */
    static seq_t *seq = NULL;
    static idx_t  len = 0;   /* Number of values currently stored (max index + 1). */
    static idx_t  cap = 0;   /* Allocated capacity. */

    /* Allocate or grow the array if needed. */
    if (len <= n)
    {
        idx_t needed = n + 1;
        if (cap < needed)
        {
            idx_t newcap = cap ? cap * 2 : 4;
            if (newcap < needed)
                newcap = needed;
            seq_t *tmp = (seq_t *)realloc(seq, (size_t)newcap * sizeof(seq_t));
            if (!tmp) abort();        /* allocation failure */
            seq = tmp;
            cap = newcap;
        }

        /* Fill in missing terms up to index n. */
        for (idx_t i = len; i <= n; ++i)
        {
            if (i == 0 || i == 1)
            {
                seq[(size_t)i] = (seq_t)0;
                continue;
            }

            idx_t idx = i - 1;               /* index of the 'last' term a(n) */
            seq_t last = seq[(size_t)idx];

            /* Count how many times 'last' occurs in a(0..idx-1) */
            idx_t count = 0;
            for (idx_t j = 0; j < idx; ++j)
                if (seq[(size_t)j] == last)
                    ++count;

            seq_t new_val;
            if (count == 0)   /* a(n) has not appeared before */
            {
                /* idx and last satisfy last <= idx, so the subtraction is safe. */
                idx_t prev_idx = idx - (idx_t)last;
                new_val = last - seq[(size_t)prev_idx];
            }
            else
            {
                new_val = (seq_t)count;
            }
            seq[(size_t)i] = new_val;
        }
        len = n + 1;
    }

    return seq[(size_t)n];
}
