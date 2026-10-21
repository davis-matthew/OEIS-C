/*
 * A280250  Sum of the smaller parts of the partitions of 2n into 2 squarefree
 *          parts.
 * https://oeis.org/A280250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{i=1..n} i * μ(i)^2 * μ(2n−i)^2 * where μ is the Möbius
 *            function, μ(m)^2 = 1 if m is squarefree, * otherwise 0. This
 *            implementation uses a precomputed table of * squarefree flags up
 *            to 2n
 * Range:     u8 n <= 33, u16 n <= 503, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static bool *sqfree = NULL;
static size_t sqfree_size = 0;

/* Allocate and fill the array sqfree[0..limit] with squarefree flags. */
static void ensure_sqfree(idx_t limit)
{
    if ((size_t)(limit + 1) <= sqfree_size)
        return;

    size_t new_size = (size_t)(limit + 1);
    bool *arr = (bool *)malloc(new_size * sizeof(bool));
    if (arr == NULL) {
        /* On allocation failure do nothing – the caller will use
         * whatever data is already present.  In practice this should
         * never happen for the limits required by the sequence. */
        return;
    }

    for (size_t k = 0; k < new_size; ++k)
        arr[k] = true;
    arr[0] = false;            /* 0 is not squarefree */

    for (uint64_t p = 2; p * p <= (uint64_t)limit; ++p) {
        uint64_t sq = p * p;
        for (uint64_t j = sq; j <= (uint64_t)limit; j += sq)
            arr[(size_t)j] = false;
    }

    free(sqfree);
    sqfree = arr;
    sqfree_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    idx_t limit = 2 * n;
    ensure_sqfree(limit);

    seq_t sum = (seq_t)0;
    for (idx_t i = 1; i <= n; ++i) {
        if (sqfree[(size_t)i] && sqfree[(size_t)(limit - i)])
            sum += (seq_t)i;
    }
    return sum;
}
