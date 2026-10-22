/*
 * A339930  a(n+1) = a(n-2-a(n)^2) + 1, starting with a(1) = a(2) = a(3) = 0.
 * https://oeis.org/A339930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+1) = a(n-2-a(n)^2) + 1, a(1) = a(2) = a(3) = 0
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n <= 3)
        return 0;

    /* Static memoisation table */
    static seq_t *table   = NULL;  /* table[0] is unused, table[1..] holds a(n) */
    static size_t  cap    = 0;     /* allocated size of table */
    static size_t  done   = 0;     /* largest index already computed */

    /* Ensure table has room for indices up to n (inclusive) */
    if (cap < n + 1) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(table, new_cap * sizeof(seq_t));
        if (!tmp)
            return 0;               /* allocation failure – graceful exit */
        /* Zero‑initialise exclusively the newly allocated part */
        if (new_cap > cap)
            for (size_t i = cap; i < new_cap; ++i)
                tmp[i] = 0;
        table = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms in increasing order */
    size_t start = (done < 3) ? 4 : done + 1;
    for (size_t i = start; i <= n; ++i) {
        seq_t prev = table[i - 1];
        seq_t sq   = prev * prev;
        size_t idx = i - 3 - (size_t)sq;    /* back reference index */
        table[i] = table[idx] + 1;
    }
    done = n;

    return table[n];
}
