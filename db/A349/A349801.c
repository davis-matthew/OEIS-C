/*
 * A349801  Number of integer partitions of n into three or more parts or into
 *          two equal parts.
 * https://oeis.org/A349801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000041(n) - ceil(n/2) (for n > 0; a(0)=0)
 * Range:     u8 n <= 16, u16 n <= 43, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *partition_cache = NULL;   /* cached partition numbers: p[0..N] */
static idx_t   cache_size     = 0;      /* number of elements currently stored */

/* Extend the partition cache to compute p[0..n] */
static void ensure_partition_cache(idx_t n)
{
    if (cache_size > n) return;                    /* already computed */

    idx_t new_size = n + 1;
    /* realloc returns void*; cast explicitly for C++ compatibility */
    partition_cache = (seq_t *)realloc(partition_cache,
                                       new_size * sizeof(seq_t));
    if (cache_size == 0) {
        partition_cache[0] = (seq_t)1;              /* p(0) = 1 */
        cache_size = 1;
    }

    for (idx_t cur = cache_size; cur <= n; ++cur) {
        seq_t sum_odd  = 0;
        seq_t sum_even = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > cur) break;

            seq_t term = partition_cache[cur - pent1];
            if (k & 1) sum_odd += term; else sum_even += term;

            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 > cur) continue;
            term = partition_cache[cur - pent2];
            if (k & 1) sum_odd += term; else sum_even += term;
        }
        /* sum_odd - sum_even gives the partition number */
        partition_cache[cur] = sum_odd - sum_even;
    }

    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* a(0) is defined as 0 */

    ensure_partition_cache(n);

    seq_t ceil_half = ((seq_t)n + 1) / 2;  /* ceil(n/2) */
    return partition_cache[n] - ceil_half;
}
