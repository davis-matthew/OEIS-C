/*
 * A374420  Triangle T(n, k) for the number of permutations of symmetric group
 *          S_n with an odd number of non-fixed point cycles, without k <= n
 *          particular fixed points.
 * https://oeis.org/A374420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * a(n) = T(r,k) * where r = floor((sqrt(8*n+1)-1)/2) - 1 // row
 *            index * k = n - r*(r+1)/2 // column index * T(n,k) is defined by
 *            the recurrence * T(n,0) = (n! + (n-2)*2^(n-1)) / 2 (for n ≥ 0) *
 *            T(n,k) = T(n,k-1) - T(n-1,k-1) (1 ≤ k ≤ n) * * The sequence a(n)
 *            is obtained by flattening the triangle...
 * Range:     u8 n <= 20, u16 n <= 44, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow2.h"   /* provides seq_t pow2(idx_t exp) */

seq_t a(idx_t n)
{
    /* Determine the row (r) and column (k) for index n. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    idx_t k = n - r * (r + 1) / 2;

    /* Handle the trivial first element. */
    if (r == 0)
        return 0;

    /* Static buffers for two rows; grow as needed. */
    static seq_t *prev = NULL;
    static seq_t *curr = NULL;
    static size_t cap = 0;

    /* Ensure sufficient capacity for current row (length r+1). */
    size_t need = (size_t)(r + 1);
    if (need > cap) {
        size_t new_cap = cap ? cap * 2 : 1;
        while (new_cap < need)
            new_cap <<= 1;
        prev = (seq_t *)realloc(prev, sizeof(seq_t) * new_cap);
        curr = (seq_t *)realloc(curr, sizeof(seq_t) * new_cap);
        cap = new_cap;
    }

    /* Initial values for row 0. */
    prev[0] = 0;   /* T(0,0) */
    seq_t fact = 1;   /* will hold i! while iterating */

    /* Build rows 1 … r. */
    for (idx_t i = 1; i <= r; ++i) {
        fact = fact * (seq_t)i;                /* i!  */
        seq_t pow_two = pow2(i - 1);           /* 2^(i-1) */

        /* T(i,0) computation. */
        seq_t T0;
        if (i <= 1)
            T0 = 0;                            /* T(1,0)=0 and T(0,0)=0 */
        else
            T0 = (fact + (pow_two * (seq_t)(i - 2))) / (seq_t)2;
        curr[0] = T0;

        /* Recurrence for k ≥ 1. */
        for (idx_t j = 1; j <= i; ++j)
            curr[j] = curr[j - 1] - prev[j - 1];

        /* If we reached the desired row, return the required element. */
        if (i == r)
            return curr[k];

        /* Prepare for the next iteration.  Swap pointers. */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    /* Should never be reached. */
    return 0;
}
