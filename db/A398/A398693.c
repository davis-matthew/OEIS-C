/*
 * A398693  Cardinality of the unique largest subdirectly irreducible factor
 *          of the lattice of partitions of n under the dominance order.
 * https://oeis.org/A398693
 *
 * Offset:    13    (a(13) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n) - 4*n + 7 - (-1)^n (where p(n) is the partition
 *            number)
 * Range:     u8 n <= 17, u16 n <= 43, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Cache for partition numbers */
static seq_t *partitions = NULL;
static idx_t cache_size = 0;   /* number of elements currently computed, i.e. highest index + 1 */
static idx_t cache_capacity = 0;

/* Ensure the cache holds values up to at least 'n' (inclusive).  */
static void ensure_cache(idx_t n)
{
    if (cache_capacity == 0) {
        /* initial allocation: size 1 for p[0] */
        cache_capacity = 1;
        cache_size = 1;
        partitions = (seq_t *)malloc(sizeof(seq_t) * cache_capacity);
        if (!partitions) exit(EXIT_FAILURE);
        partitions[0] = 1;   /* p(0) = 1 */
    }

    if (n < cache_size) return;   /* already computed */

    /* Grow cache if necessary */
    if (n + 1 > cache_capacity) {
        idx_t new_cap = cache_capacity;
        while (new_cap <= n) new_cap <<= 1;   /* double until enough */
        seq_t *tmp = (seq_t *)realloc(partitions, sizeof(seq_t) * new_cap);
        if (!tmp) exit(EXIT_FAILURE);
        partitions = tmp;
        cache_capacity = new_cap;
    }

    /* Compute p(m) for m = cache_size .. n */
    for (idx_t m = cache_size; m <= n; ++m) {
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > m) break;
            /* sign: + for odd k, - for even k */
            if (k & 1)  sum_pos += partitions[m - pent1];
            else        sum_neg += partitions[m - pent1];

            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 > m) continue;
            if (k & 1)  sum_pos += partitions[m - pent2];
            else        sum_neg += partitions[m - pent2];
        }
        partitions[m] = sum_pos - sum_neg;   /* safe: sum_pos >= sum_neg */
    }
    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    if (n < 13) {
        /* Value for n < 13 is irrelevant; compute anyway. */
    }

    /* Ensure partition numbers are available */
    ensure_cache(n);

    seq_t pn = partitions[n];
    seq_t four_n = (seq_t)4 * (seq_t)n;

    /* Compute pn - 4*n + (n even? 6 : 8)  */
    seq_t res = pn - four_n;
    if (n & 1)
        res += (seq_t)8;
    else
        res += (seq_t)6;

    return res;
}
