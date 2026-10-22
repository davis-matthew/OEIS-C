/*
 * A326465  Sum of the smallest parts of the partitions of n into 9 parts.
 * https://oeis.org/A326465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{q=1..⌊n/9⌋} q * P₈(n-9q)
 * Range:     u8 n <= 25, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where P₈(m) is the number of partitions of m into at most 8 parts   */
/* (equivalently, partitions of m with largest part ≤8).               */

#include <stdlib.h>

static seq_t *p8_cache = NULL;   /* number of partitions of m using parts ≤8 */
static size_t p8_cap  = 0;
static seq_t *a_cache = NULL;     /* cached a(n) values */
static idx_t  a_cap   = 0;        /* one past last computed index */

static void compute_tables(idx_t up_to)
{
    /* Build partition table up to 'up_to' (parts ≤8) */
    if ((size_t)up_to + 1 > p8_cap) {
        size_t new_cap = (size_t)up_to + 1;
        seq_t *new_p = (seq_t *)realloc(p8_cache, new_cap * sizeof(seq_t));
        if (!new_p) exit(EXIT_FAILURE);  /* allocation failed */
        p8_cache = new_p;
        /* Initialize array to zeros and set p8_cache[0] = 1 */
        for (size_t i = 0; i < new_cap; ++i) p8_cache[i] = 0;
        p8_cache[0] = 1;
        /* Classic coin–change DP with coins 1..8 */
        for (idx_t coin = 1; coin <= 8; ++coin)
            for (size_t m = coin; m < new_cap; ++m)
                p8_cache[m] += p8_cache[m - coin];
        p8_cap = new_cap;
    }

    /* Compute a[n] for all n up to 'up_to' */
    if ((size_t)up_to + 1 > a_cap) {
        size_t new_a_cap = (size_t)up_to + 1;
        seq_t *new_a = (seq_t *)realloc(a_cache, new_a_cap * sizeof(seq_t));
        if (!new_a) exit(EXIT_FAILURE);
        a_cache = new_a;
        for (idx_t i = a_cap; i <= up_to; ++i) {
            seq_t sum = 0;
            idx_t max_q = i / 9;
            for (idx_t q = 1; q <= max_q; ++q)
                sum += (seq_t)q * p8_cache[i - 9 * q];
            a_cache[i] = sum;
        }
        a_cap = new_a_cap;
    }
}

seq_t a(idx_t n)
{
    compute_tables(n);
    return a_cache[n];
}
