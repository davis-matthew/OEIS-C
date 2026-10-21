/*
 * A276428  Sum over all partitions of n of the number of distinct parts i of
 *          multiplicity i.
 * https://oeis.org/A276428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i>=1, i^2<=n} [ p(n-i^2) - p(n-i^2-i) ] * where p(k)
 *            is the ordinary partition number of k (p(0)=1, p(k<0)=0). * The
 *            implementation precomputes p[0..max_n] on-demand with a *
 *            classic O(max_n^2) DP. For each query it sums the required terms
 *            * using the precomputed array. All arithmetic...
 * Range:     u8 n <= 20, u16 n <= 48, u32 n <= 136, u64 n <= 430,
 *            u128 n <= 1482, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* allocate or grow an array of seq_t to hold partition numbers up to max_n.
 * The array is kept static and rebuilt only when a larger max_n is requested. */
static seq_t *partitions = NULL;
static idx_t partition_cap = 0;   /* size of the array, i.e. partition_cap-1 is the largest index available */

/* ensure partitions[0..max_n] are valid.  Recompute from scratch when enlarging. */
static void ensure_partitions(idx_t max_n)
{
    if (max_n + 1 <= partition_cap)
        return;                           /* already enough */

    /* allocate new array */
    size_t new_cap = (size_t)(max_n + 1);
    seq_t *tmp = (seq_t *)realloc(partitions, new_cap * sizeof(seq_t));
    if (tmp == NULL) exit(EXIT_FAILURE);
    partitions = tmp;
    partition_cap = new_cap;

    /* initialise to 0 */
    for (idx_t i = 0; i < partition_cap; ++i)
        partitions[i] = 0;
    partitions[0] = 1;                   /* p(0) = 1 */

    /* classic partition DP: for each coin size from 1 to max_n,
       add its contribution to all larger totals. */
    for (idx_t coin = 1; coin <= max_n; ++coin) {
        for (idx_t val = coin; val <= max_n; ++val) {
            partitions[val] += partitions[val - coin];
        }
    }
}

/* compute a(n) as defined above. */
seq_t a(idx_t n)
{
    /* produce partition numbers up to n, sufficient for all n-i^2 and n-i^2-i values. */
    ensure_partitions(n);

    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        idx_t idx1 = n - i * i;           /* n - i^2 */
        seq_t term = partitions[idx1];

        idx_t idx2 = n - i * i - i;       /* n - i^2 - i */
        if (idx2 <= idx1) /* idx2 >=0 because i* (i+1) <= i*i + i <= n when inside loop */
            term -= partitions[idx2];

        sum += term;
    }
    return sum;
}
