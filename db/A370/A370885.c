/*
 * A370885  Irregular triangle read by rows: T(n,k) is the total number of
 *          unmatched parentheses (both left and right) in the k-th string of
 *          parentheses of length n, where strings within a row are in reverse
 *          lexicographical order.
 * https://oeis.org/A370885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 9449, u16 n <= 9449, u32 n <= 9449, u64 n <= 9449,
 *            u128 n <= 9449, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A370885: Sequence obtained by concatenating the rows of the triangle
   T(n,k), where T(n,k) is the total number of unmatched parentheses
   in the k‑th string of parentheses of length n (reverse lexicographic
   order).  The function returns the nth term of this sequence. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute the row r such that 2^r - 1 <= n < 2^(r+1) - 1   */
    idx_t idx = n + 1;           /* n + 1 is the number of entries up to row r inclusive */
    idx_t r = 0;
    if (idx > 1) {
        /* Find the index of the highest set bit: r = floor(log2(idx)) */
        idx_t tmp = idx;
        while (tmp > 1) {
            tmp >>= 1;
            ++r;
        }
    }

    /* Offset within the row : k = n - (2^r - 1) */
    idx_t start = (idx_t)1 << r;          /* 2^r   */
    start -= 1;                           /* 2^r - 1 */
    idx_t k = n - start;                  /* 0 <= k < 2^r */

    /* The reverse lexicographic order corresponds to descending binary
       numbers from (2^r - 1) downwards.  The k‑th string in that order
       therefore has binary value: bits = (2^r - 1) - k. */
    idx_t bits_val = ((idx_t)1 << r) - 1 - k;

    /* Count unmatched parentheses in the string described by bits_val */
    idx_t depth = 0;                    /* number of unmatched '(' seen so far */
    idx_t unmatched_right = 0;          /* count of ')' that had no matching '(' */
    for (idx_t i = r; i > 0; --i) {
        idx_t bit = (bits_val >> (i - 1)) & 1; /* 0 -> '(', 1 -> ')' */
        if (bit == 0) {
            ++depth;
        } else {
            if (depth > 0) {
                --depth;               /* match a '(' */
            } else {
                ++unmatched_right;     /* unmatched ')' */
            }
        }
    }
    return (seq_t)(depth + unmatched_right);   /* total unmatched parentheses */
}
