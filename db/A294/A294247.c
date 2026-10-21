/*
 * A294247  Sum of the parts in the partitions of n into exactly two distinct
 *          squarefree parts.
 * https://oeis.org/A294247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * Sum_{i=1..floor((n-1)/2)} mu(i)^2 * mu(n-i)^2
 * Range:     u8 n <= 31, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 2)
        return (seq_t)0;

    static idx_t max_n = 0;
    static seq_t *sf = NULL;

    if (n > max_n) {
        idx_t new_cap = n + 1;
        seq_t *new_sf = (seq_t *)calloc(new_cap, sizeof(seq_t));
        if (!new_sf)
            return (seq_t)0;  /* allocation failure – unlikely here */

        for (idx_t i = 0; i < new_cap; ++i)
            new_sf[i] = (seq_t)1;
        new_sf[0] = 0;  /* 0 is not squarefree */

        for (idx_t i = 2; i <= n / i; ++i) {
            idx_t sq = i * i;
            for (idx_t j = sq; j <= n; j += sq)
                new_sf[j] = 0;
        }

        free(sf);
        sf = new_sf;
        max_n = n;
    }

    seq_t count = 0;
    idx_t limit = (n - 1) / 2;
    for (idx_t i = 1; i <= limit; ++i) {
        if (sf[i] && sf[n - i])
            ++count;
    }

    return (seq_t)n * count;
}
