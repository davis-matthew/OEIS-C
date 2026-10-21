/*
 * A281669  Expansion of Sum_{i>=1} x^(i^3)/(1 + x^(i^3)) * Product_{j>=1} (1
 *          + x^(j^3)).
 * https://oeis.org/A281669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = total number of parts in all partitions of n into
 *            distinct cubes (OEIS A281669)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* not used but harmless */

    /* static cache: computed up to cache_n */
    static idx_t cache_n = 0;
    static seq_t *cnt = NULL;    /* number of partitions */
    static seq_t *parts = NULL;  /* total parts in those partitions */

    if (n > cache_n)  /* need to recompute up to n */
    {
        /* allocate new arrays */
        idx_t new_n = n;
        seq_t *new_cnt = (seq_t *)calloc((size_t)(new_n + 1), sizeof(seq_t));
        seq_t *new_parts = (seq_t *)calloc((size_t)(new_n + 1), sizeof(seq_t));

        if (!new_cnt || !new_parts)  /* allocation failure */
        {
            /* fall back to zero */
            free(new_cnt);
            free(new_parts);
            return (seq_t)0;
        }

        /* base case: empty partition */
        new_cnt[0] = (seq_t)1;
        new_parts[0] = (seq_t)0;

        /* dynamic programming over distinct cubes */
        for (idx_t i = 1; ; ++i)
        {
            idx_t cube = i * i * i;
            if (cube > new_n) break;
            /* process this cube */
            for (idx_t s = new_n; s >= cube; --s)
            {
                seq_t part_before = new_cnt[s - cube];
                seq_t parts_before = new_parts[s - cube];

                new_cnt[s] = new_cnt[s] + part_before;
                new_parts[s] = new_parts[s] + parts_before + part_before;
            }
        }

        /* free old cache */
        free(cnt);
        free(parts);

        /* store new cache */
        cnt = new_cnt;
        parts = new_parts;
        cache_n = new_n;
    }

    return parts[n];
}
