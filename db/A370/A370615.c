/*
 * A370615  a(0) = 0. For n >= 1, a(n) = 1 + (sum of the next consecutive
 *          a(n-1) positive integers).
 * https://oeis.org/A370615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0. For n >= 1, a(n) = 1 + sum of the next a(n-1)
 *            consecutive positive integers. The start of each block is the
 *            integer immediately following the last integer used in the
 *            previous block
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *seq = NULL;          /* cache of computed terms */
static size_t cap = 0;             /* capacity of seq */
static idx_t  computed = 0;        /* highest index already computed */
static seq_t  next_start = 1;      /* first integer of the next block */

seq_t a(idx_t n)
{
    /* initialize cache if needed */
    if (seq == NULL) {
        /* allocate space for at least one term (a(0)) */
        cap = 1;
        seq = (seq_t *)malloc(cap * sizeof(seq_t));
        if (seq == NULL) return 0;          /* allocation failed */
        seq[0] = 0;                         /* a(0) = 0 */
    }

    /* compute terms up to n if not already done */
    if (n > computed) {
        /* ensure capacity */
        if ((size_t)(n + 1) > cap) {
            size_t newCap = cap > 0 ? cap * 2 : 1;
            while (newCap <= (size_t)(n + 1))
                newCap *= 2;
            seq_t *tmp = (seq_t *)realloc(seq, newCap * sizeof(seq_t));
            if (tmp == NULL)
                return 0;                   /* memory allocation failed */
            seq = tmp;
            cap = newCap;
        }

        for (idx_t k = computed + 1; k <= n; ++k) {
            seq_t aPrev = seq[k - 1];
            /* sum of aPrev consecutive integers starting at next_start:
               aPrev * (2*next_start + aPrev - 1) / 2 */
            seq_t sum = aPrev * (((seq_t)2 * next_start) + aPrev - (seq_t)1) / 2;
            seq_t aCurr = sum + 1;
            seq[k] = aCurr;
            next_start += aPrev;           /* prepare start for next block */
        }
        computed = n;
    }

    return seq[n];
}
