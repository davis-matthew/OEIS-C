/*
 * A364027  a(0) = 0, a(1) = 0; for n > 1, a(n) is the number of pairs of
 *          consecutive terms that sum to the same value as a(n-2) + a(n-1).
 * https://oeis.org/A364027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of consecutive pairs of terms that sum to *
 *            a(n-2)+a(n-1). a(0)=0, a(1)=0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *seq = NULL;        /* array of computed terms          */
static idx_t  cap  = 0;           /* allocated size of seq array      */
static idx_t  last = 0;           /* index of the largest computed term */

/* Ensure the sequence array can hold indices up to n.
 * The array is grown only when needed. */
static void ensure_capacity(idx_t n)
{
    if (n < cap)
        return;
    /* Allocate space for at least (n+1) elements. */
    seq_t *tmp = (seq_t *)realloc(seq, (n + 1) * sizeof(seq_t));
    if (!tmp) {
        /* Allocation failure: abort (no error handling required). */
        abort();
    }
    seq = tmp;
    cap = n + 1;
}

seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == 0 || n == 1)
        return (seq_t)0;

    /* First-time initialization. */
    if (seq == NULL) {
        ensure_capacity(1);          /* at least indices 0 and 1 */
        seq[0] = (seq_t)0;
        seq[1] = (seq_t)0;
        last   = 1;
    }

    /* If we already have a(n), just return it. */
    if (n <= last)
        return seq[n];

    /* Extend the computation up to n. */
    for (idx_t cur = last + 1; cur <= n; ++cur) {
        /* Sum of the two most recent terms. */
        seq_t target = seq[cur - 2] + seq[cur - 1];

        /* Count how many consecutive pairs among 0..cur-2 sum to target. */
        idx_t count = 0;
        for (idx_t i = 0; i + 1 < cur; ++i) {
            if ((seq[i] + seq[i + 1]) == target)
                ++count;
        }

        /* Record the new term, ensuring the array is large enough. */
        ensure_capacity(cur);
        seq[cur] = (seq_t)count;
    }

    last = n;
    return seq[n];
}
