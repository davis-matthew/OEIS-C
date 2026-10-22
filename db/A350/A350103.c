/*
 * A350103  Triangle read by rows. Number of self-measuring subsets of the
 *          initial segment of the natural numbers strictly below n and
 *          cardinality k. Number of subsets S of [n] with S = distset(S) and
 *          |S| = k.
 * https://oeis.org/A350103
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   For index n, let r be the unique integer such that r*(r+1)/2 ≤ n
 *            < (r+1)*(r+2)/2. Let c = n - r*(r+1)/2. Then the triangle entry
 *            is a(n) = 1 if c ≤ 1 floor((r-1)/(c-1)) otherwise
 * Range:     u8 n <= 9137, u16 n <= 9137, u32 n <= 9137, u64 n <= 9137,
 *            u128 n <= 9137, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* external linkage as required */
seq_t a(idx_t n) {
    /* find r such that t_r ≤ n < t_{r+1} using a small linear scan;
       r is at most about sqrt(2n) which is tiny for n ≤ 10^4. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }

    /* triangular number for current r */
    idx_t t = r * (r + 1) / 2;
    idx_t c = n - t;            /* column index within the row */

    if (c <= 1) {
        return (seq_t)1;
    } else {
        /* r ≥ 2 and c ≥ 2 here */
        seq_t num = (seq_t)(r - 1);
        seq_t den = (seq_t)(c - 1);
        return num / den;
    }
}
