/*
 * A286108  Square array read by antidiagonals: A(n,k) = T(2*(n AND k), n XOR
 *          k), where T(n,k) is sequence A001477 considered as a
 *          two-dimensional table, AND is bitwise-and (A004198) and XOR is
 *          bitwise-xor (A003987).
 * https://oeis.org/A286108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(2*(i AND j), i XOR j) where (i,j) are the coordinates
 *            obtained by reading the square array by descending
 *            antidiagonals. Using the identity T(n,k) = t(n+k)+n we show that
 *            a(n) = t(d) + 2*(i & j) (OEIS formula) with d = ⌊(√(8n+1)-1)/2⌋,
 *            i = n - t(d), j = d - i, t(m) = m*(m+1)/2
 * Range:     u8 n <= 255, u16 n <= 9862, u32 n <= 9862, u64 n <= 9862,
 *            u128 n <= 9862, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return the greatest integer d such that d*(d+1)/2 ≤ n. */
static idx_t diag_index(idx_t n)
{
    idx_t low = 0, high = 1;
    /* expand upper bound until it exceeds n */
    while (((unsigned __int128)high * (high + 1) / 2) <= n)
        high <<= 1;
    /* binary search on [low, high] */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* diagonal index */
    idx_t d = diag_index(n);

    /* triangular number t(d) */
    idx_t td_idx = (idx_t)(((unsigned __int128)d * (d + 1)) / 2);
    seq_t td = ((seq_t)d * (d + 1)) / 2;

    /* coordinates of n on the antidiagonal */
    idx_t i = n - td_idx;
    idx_t j = d - i;

    /* common part of the formula */
    idx_t c = i & j;

    /* final value: t(d) + 2*(i & j) */
    return td + (seq_t)(2 * c);
}
