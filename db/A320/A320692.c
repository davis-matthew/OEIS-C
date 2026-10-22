/*
 * A320692  Number of partitions of n with up to five distinct kinds of 1.
 * https://oeis.org/A320692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n) + 4*p(n-1) + 5*p(n-2) - 5*p(n-4) - 4*p(n-5) -
 *            p(n-6), where p(k) is the ordinary partition number of k (p(k)=0
 *            for k<0)
 * Range:     u8 n <= 11, u16 n <= 36, u32 n <= 119, u64 n <= 408,
 *            u128 n <= 1459, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The ordinary partition numbers are computed once and stored
   in a growing static array.  The algorithm uses the
   Generalised Pentagonal Number Theorem.  For an arbitrary-
   precision build the sequence type seq_t has proper
   copy/assignment, so a manual element-wise copy is used
   instead of memcpy. */

#include <stdlib.h>

/* static storage for partition numbers p(0)..p(n) */
static seq_t *partitions = NULL;      /* array of computed partitions   */
static size_t partitions_cap = 0;     /* number of elements currently stored */

/* Ensure that partitions[0 .. n] are computed.  */
static void ensure_partitions(idx_t n)
{
    if (partitions_cap > (size_t)n)        /* already computed up to n */
        return;

    /* grow the array to size n+1 */
    size_t new_cap = (size_t)n + 1;
    seq_t *new_arr = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_arr)                          /* allocation failure – caller will
                                                fail naturally */
        return;

    /* copy any existing data correctly, element by element */
    if (partitions) {
        for (idx_t i = 0; i < partitions_cap; ++i)
            new_arr[i] = partitions[i];
        free(partitions);
    }

    /* initialise first element */
    new_arr[0] = (seq_t)1;                 /* p(0) = 1 */

    /* compute partition numbers from current size up to n */
    idx_t start = (partitions_cap == 0) ? 0 : (idx_t)partitions_cap;
    for (idx_t i = start; i <= n; ++i) {
        if (i == 0)                         /* already set p(0) */
            continue;

        seq_t pos = 0, neg = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > i)
                break;

            if (k & 1)
                pos += new_arr[i - g1];
            else
                neg += new_arr[i - g1];

            idx_t g2 = k * (3 * k + 1) / 2;
            if (g2 > i)
                continue;

            if (k & 1)
                pos += new_arr[i - g2];
            else
                neg += new_arr[i - g2];
        }

        new_arr[i] = pos - neg;
    }

    partitions = new_arr;
    partitions_cap = new_cap;
}

/* Public function required by the framework */
seq_t a(idx_t n)
{
    /* compute all needed partitions */
    ensure_partitions(n);

    seq_t result = partitions[n];

    if (n >= 1)
        result += partitions[n - 1] * (seq_t)4;
    if (n >= 2)
        result += partitions[n - 2] * (seq_t)5;

    if (n >= 4)
        result -= partitions[n - 4] * (seq_t)5;
    if (n >= 5)
        result -= partitions[n - 5] * (seq_t)4;
    if (n >= 6)
        result -= partitions[n - 6];

    return result;
}
