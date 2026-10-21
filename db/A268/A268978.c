/*
 * A268978  Triangle T(n,k) read by rows with 1 <= k <= n: number of entries
 *          in the first n rows of Pascal's triangle that are divisible by k.
 * https://oeis.org/A268978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 253, u16 n <= 9918, u32 n <= 9918, u64 n <= 9918,
 *            u128 n <= 9918, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A268978: triangular array T(n,k) read by rows.
 *  T(n,k) = number of binomial coefficients C(i,j) with 0 ≤ i < n
 *  that are divisible by k (1 ≤ k ≤ n).
 *  The flattened sequence a(n) lists the rows of this triangle
 *  in order, i.e. a(1)=T(1,1), a(2)=T(2,1), a(3)=T(2,2), ...
 *  This function finds the appropriate (n,k) pair for the given
 *  flattened index n and counts the entries divisible by k.
 */

#include <stdlib.h>

/*  Find the smallest r such that r*(r+1)/2 >= n.  */
static idx_t find_row(idx_t n)
{
    idx_t r = 0;
    while ((r * (r + 1)) / 2 < n)
        ++r;
    return r;
}

seq_t a(idx_t n)
{
    /* Determine row r and column k in the triangular array.  */
    idx_t r = find_row(n);
    idx_t k = n - ((r - 1) * r) / 2;      /* 1‑based column index, 1 ≤ k ≤ r */

    /* Special case k = 1: all binomial coefficients are divisible by 1. */
    if (k == 1)
        return (seq_t)r * ((seq_t)r + 1) / 2;

    /* Count coefficients divisible by k in rows 0 .. r-1.  */
    seq_t count = 0;

    /* Allocate two rows for Pascal's triangle modulo k.  */
    idx_t *prev = (idx_t *)calloc(r + 1, sizeof(idx_t));
    idx_t *next = (idx_t *)calloc(r + 1, sizeof(idx_t));
    if (!prev || !next) {
        free(prev);
        free(next);
        return (seq_t)0;   /*  Out of memory – should never happen.   */
    }

    /* Row 0: only the element 1.  */
    prev[0] = 1 % k;
    if (prev[0] == 0) ++count;

    /* Generate rows 1 .. r-1 inclusive and count zeros.  */
    for (idx_t i = 1; i < r; ++i) {
        /* First and last elements of the row are 1 (mod k).  */
        next[0] = 1 % k;
        next[i] = 1 % k;
        if (next[0] == 0) ++count;
        if (next[i] == 0 && i != 0) ++count;

        /* Interior elements: sum of two elements above modulo k.  */
        for (idx_t j = 1; j < i; ++j) {
            idx_t val = prev[j - 1] + prev[j];
            if (val >= k) val -= k;     /* equivalent to val %= k for small ints */
            next[j] = val;
            if (val == 0) ++count;
        }

        /* Prepare for next iteration: swap prev and next.  */
        idx_t *tmp = prev;
        prev = next;
        next = tmp;
    }

    free(prev);
    free(next);
    return count;
}
