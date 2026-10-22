/*
 * A332046  a(n) is the smallest positive integer such that there exist
 *          exactly n positive integers less than a(n) whose digital sum in
 *          base 10 is equal to the digital sum of a(n).
 * https://oeis.org/A332046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = smallest positive integer m such that exactly n
   smaller integers have the same decimal digit sum as m. */

#include <stdlib.h>
#include <stdbool.h>

/* Compute a(n) for n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined for the offset */

    /* Array of counts of occurrences of each digit sum seen so far. */
    size_t cap = 64;                         /* initial capacity for sums */
    idx_t *cnt = (idx_t *)calloc(cap, sizeof(idx_t));
    if (!cnt) abort();                       /* out of memory */

    idx_t m = 1;                              /* current integer to test */

    for (;; ++m)
    {
        /* Calculate the decimal digit sum of m.  The loop uses only
           integer operations, so it works regardless of the concrete
           representation of seq_t. */
        idx_t sum = 0;
        idx_t t = m;
        while (t > 0)
        {
            sum += t % 10;
            t /= 10;
        }

        /* Grow the count array if a larger digit sum is observed. */
        if (sum >= cap)
        {
            size_t newcap = sum + 1;
            idx_t *tmp = (idx_t *)realloc(cnt, newcap * sizeof(idx_t));
            if (!tmp) { free(cnt); abort(); }
            /* Zero‑initialize any new slots. */
            for (size_t i = cap; i < newcap; ++i) tmp[i] = 0;
            cnt = tmp;
            cap = newcap;
        }

        /* If exactly n numbers below m share this digit sum, m is a(n). */
        if (cnt[sum] == n)
        {
            free(cnt);
            return (seq_t)m;
        }

        /* Record that m contributes to this digit sum. */
        ++cnt[sum];
    }
}
