/*
 * A317948  An example of a morphic word: the sorted (by length, then
 *          alphabetically) sequence of words of the form a*b* under the
 *          action of a finite automaton defined as follows: start state is 0;
 *          a and b map states [0, 1, 2, 3] to states [1, 2, 3, 0] and [0, 3,
 *          1, 2], respectively.
 * https://oeis.org/A317948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = S(m,k) where S(m,k) = 0 if m % 4 == 0 (((m % 4) - k - 1)
 *            mod 3) + 1 otherwise (m,k) are obtained from the antidiagonal
 *            representation of n: let d be the unique integer with d*(d+1)/2
 *            < n ≤ (d+1)*(d+2)/2 offset = n - d*(d+1)/2 - 1 (0 ≤ offset ≤ d)
 *            k = offset, m = d - k Thus a(n) = S(d-k,k)....
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   tri1
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/tri1.h"

seq_t a(idx_t n)
{
    /* Find d such that tri1(d-1) < n ≤ tri1(d). */
    idx_t low = 0, high = 1;
    /* Expand high until it satisfies the inequality. */
    while (tri1(high) < (unsigned __int128)n)
        high <<= 1;

    /* Binary search for the minimal d with tri1(d) >= n. */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (tri1(mid) >= (unsigned __int128)n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t d = low;                     /* anti‑diagonal index */

    /* Offset within the diagonal (k). */
    unsigned __int128 tri_before = (unsigned __int128)d * (d + 1) / 2;
    idx_t offset = n - (idx_t)tri_before - 1;   /* 0 … d */
    idx_t k = offset;
    idx_t m = d - k;                     /* leading a's */

    /* Compute the final state of the automaton. */
    if ((m & 3U) == 0)            /* m % 4 == 0 */
        return 0;

    int t   = (int)(m & 3U);          /* m % 4, value 1‑3 */
    int km  = (int)(k % 3U);          /* k % 3, value 0‑2 */

    int diff = t - 1 - km;            /* may be negative */
    diff %= 3;
    if (diff < 0)
        diff += 3;

    return (seq_t)(diff + 1);         /* 1‑3 */
}
