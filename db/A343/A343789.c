/*
 * A343789  Number of ordered partitions of an n-set without blocks of size 6.
 * https://oeis.org/A343789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of ordered partitions of an n‑set that
 *            contain no block of size 6. It satisfies the recurrence a(0)=1
 *            and for n≥1 a(n) = Σ_{k=1}^{n, k≠6} binom(n,k) * a(n−k) (∗)
 *            where binom(n,k) is the binomial coefficient. This recurrence
 *            follows from the e.g.f. 1/(2 + x⁶/6! – exp(x))
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Memoised table of values 0…max_cached.  The table is resized
       only when a larger k is requested. */
    static seq_t *table = NULL;
    static idx_t  cap   = 0;          /* |table| */

    /* ------------------------------------------------------------------
       Grow the table if necessary, filling new entries using recurrence (*)
       ------------------------------------------------------------------ */
    if (n >= cap) {
        /* Allocate at least (n+1) slots */
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(table, new_cap * sizeof(seq_t));
        if (!tmp) abort();  /* memory allocation failed */
        table = tmp;

        /* initialise the only required base term */
        if (cap == 0)
            table[0] = (seq_t)1;

        /* compute terms from old cap to the requested n */
        for (idx_t cur = cap; cur <= n; ++cur)
        {
            if (cur == 0) continue;   /* already set */

            seq_t sum   = (seq_t)0;
            seq_t binom = (seq_t)1;   /* binom(cur,0) */

            for (idx_t k = 1; k <= cur; ++k)
            {
                /* binom(cur,k) = binom(cur,k-1) * (cur-k+1) / k */
                binom = binom * (seq_t)(cur - k + 1) / (seq_t)k;

                if (k == 6) continue;        /* exclude block size 6 */
                sum += table[cur - k] * binom;
            }
            table[cur] = sum;
        }
        cap = n + 1;
    }

    return table[n];
}
