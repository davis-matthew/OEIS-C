/*
 * A398990  The binomial matrix of the partition numbers. Square array read by
 *          ascending antidiagonals.
 * https://oeis.org/A398990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = value from the binomial matrix of partition numbers, read
 *            by ascending antidiagonals
 * Range:     i8 n <= 20, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *partition_cache = NULL; /* cached partition numbers P(0), P(1), ... */
static idx_t  partition_count = 0;     /* number of entries stored */

/* Extend partition_cache to contain values up to n_needed (inclusive). */
static void ensure_partitions(idx_t n_needed)
{
    if (partition_cache == NULL || n_needed >= partition_count)
    {
        idx_t new_size = n_needed + 1;
        seq_t *new_cache = (seq_t*)malloc(new_size * sizeof(seq_t));
        if (partition_cache == NULL)          /* first time allocation */
        {
            new_cache[0] = 1;                 /* P(0) = 1 */
            for (idx_t n = 1; n < new_size; ++n)
            {
                seq_t sum = 0;
                for (idx_t k = 1; ; ++k)
                {
                    idx_t g1 = k * (3 * k - 1) / 2;
                    if (g1 > n) break;
                    int sign = (k & 1) ? 1 : -1;
                    sum += (sign == 1) ? new_cache[n - g1] : -new_cache[n - g1];
                    idx_t g2 = k * (3 * k + 1) / 2;
                    if (g2 > n) continue;
                    sum += (sign == 1) ? new_cache[n - g2] : -new_cache[n - g2];
                }
                new_cache[n] = sum;
            }
        }
        else                                 /* extend existing cache */
        {
            memcpy(new_cache, partition_cache, partition_count * sizeof(seq_t));
            for (idx_t n = partition_count; n < new_size; ++n)
            {
                seq_t sum = 0;
                for (idx_t k = 1; ; ++k)
                {
                    idx_t g1 = k * (3 * k - 1) / 2;
                    if (g1 > n) break;
                    int sign = (k & 1) ? 1 : -1;
                    sum += (sign == 1) ? new_cache[n - g1] : -new_cache[n - g1];
                    idx_t g2 = k * (3 * k + 1) / 2;
                    if (g2 > n) continue;
                    sum += (sign == 1) ? new_cache[n - g2] : -new_cache[n - g2];
                }
                new_cache[n] = sum;
            }
        }
        free(partition_cache);
        partition_cache = new_cache;
        partition_count = new_size;
    }
}

/* Main sequence function as required. */
seq_t a(idx_t n)
{
    /* Find the diagonal d such that the index n lies in the block of size d+1. */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= n) ++d;

    idx_t offset = n - d * (d + 1) / 2;   /* position inside the diagonal */
    idx_t i = d - offset;                 /* row index */
    idx_t j = offset;                     /* column index */

    idx_t p    = (i < j) ? i : j;         /* min(i,j) */
    idx_t diff = (i < j) ? (j - i) : (i - j); /* |i-j| */
    int   s    = (j <= i) ? 1 : -1;       /* sign factor */

    /* Ensure we have partition numbers up to p+diff. */
    ensure_partitions(p + diff);

    /* Temporary array for the recurrence, size diff+1. */
    enum { MAX_D = 512 };                 /* safe upper bound for typical use */
    seq_t arr[MAX_D];
    if (diff >= MAX_D)                    /* fall back to dynamic allocation if needed */
    {
        seq_t *dyn = (seq_t*)malloc((diff + 1) * sizeof(seq_t));
        for (idx_t k = 0; k <= diff; ++k) dyn[k] = partition_cache[p + k];
        for (idx_t t = 0; t < diff; ++t)
            for (idx_t k = 0; k < diff - t; ++k)
                if (s == 1) dyn[k] += dyn[k + 1];
                else       dyn[k] = dyn[k + 1] - dyn[k];
        seq_t result = dyn[0];
        free(dyn);
        return result;
    }

    for (idx_t k = 0; k <= diff; ++k) arr[k] = partition_cache[p + k];

    for (idx_t t = 0; t < diff; ++t)
        for (idx_t k = 0; k < diff - t; ++k)
            if (s == 1) arr[k] += arr[k + 1];
            else       arr[k] = arr[k + 1] - arr[k];

    return arr[0];
}
