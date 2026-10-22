/*
 * A347548  Number of partitions of n into 3 or more distinct parts.
 * https://oeis.org/A347548
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000009(n) – floor((n + 1)/2)
 * Range:     u8 n <= 29, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = Number of partitions of n into 3 or more distinct parts */
/* Offset: 6  (a(6) is the first term of the sequence) */

#include <stdlib.h>

/* Cached values of the number of partitions of n into distinct parts. */
static seq_t *part_distinct = NULL;
static idx_t max_cached_n = 0;

seq_t a(idx_t n)
{
    /* The specification only requires correct results for n >= 6. */
    if (n < 6)
        return 0;  /* behaviour for n < 6 is arbitrary */

    if (n > max_cached_n) {
        /* Reallocate cache to hold values up to n. */
        seq_t *new_cache = (seq_t *)realloc(part_distinct,
                                            (n + 1) * sizeof(seq_t));
        if (!new_cache)
            exit(1);          /* abort on allocation failure */

        part_distinct = new_cache;

        /* Initialise all entries to zero up to n. */
        for (idx_t i = 0; i <= n; ++i)
            part_distinct[i] = 0;
        part_distinct[0] = 1;   /* empty partition */

        /* Classic DP for distinct parts:
           For each possible part size k, update sums descending */
        for (idx_t k = 1; k <= n; ++k) {
            for (idx_t sum = n; sum >= k; --sum)
                part_distinct[sum] += part_distinct[sum - k];
        }

        max_cached_n = n;
    }

    /* a(n) = number of distinct partitions of n
       minus the number of partitions into 1 or 2 parts
       (which equals floor((n+1)/2)). */
    seq_t floor_half = ((seq_t)n + 1) / 2;
    return part_distinct[n] - floor_half;
}
