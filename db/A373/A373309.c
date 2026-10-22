/*
 * A373309  Number of binary partitions of n into three kinds of parts.
 * https://oeis.org/A373309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(n) = Σ_{i=0}^{⌊n/2⌋} ( (n-2i+1)*(n-2i+2)/2 ) * a(i)
 *            for n≥1
 * Range:     u8 n <= 7, u16 n <= 21, u32 n <= 87, u64 n <= 504,
 *            u128 n <= 4922, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Dynamic cache of previously computed values */
    static seq_t *table = NULL;
    static idx_t size = 0;   /* number of terms stored = size of table */

    /* Grow cache if needed */
    if (n >= size)
    {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(table, new_size * sizeof(seq_t));
        if (!tmp) abort();
        table = tmp;

        /* Initialise first term if table was empty */
        if (size == 0) {
            table[0] = (seq_t)1;
            size = 1;
        }

        /* Compute missing terms */
        for (idx_t m = size; m <= n; ++m)
        {
            /* Decide whether to use 128‑bit accumulation (seq_t is small) */
            int use128 = (sizeof(seq_t) <= 8);

            if (use128) {
                unsigned __int128 sum = 0;
                for (idx_t i = 0; i <= (m >> 1); ++i) {
                    idx_t t = m - (i << 1);                     /* t = m-2i */
                    seq_t coeff = (seq_t)(((t + 1) * (t + 2)) / 2); /* binomial(t+2,2) */
                    unsigned __int128 term = (unsigned __int128)coeff *
                                            (unsigned __int128)table[i];
                    sum += term;
                }
                table[m] = (seq_t)sum;
            } else {
                seq_t sum = 0;
                for (idx_t i = 0; i <= (m >> 1); ++i) {
                    idx_t t = m - (i << 1);
                    seq_t coeff = (seq_t)(((t + 1) * (t + 2)) / 2);
                    sum += coeff * table[i];
                }
                table[m] = sum;
            }
        }
        size = n + 1;
    }

    return table[n];
}
