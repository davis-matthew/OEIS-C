/*
 * A389737  Number of subsets of {1..n} forming a finite arithmetic
 *          progression.
 * https://oeis.org/A389737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + n + Σ_{i=1}^{n-1} Σ_{d=1}^{i} ⌊i/d⌋ (i.e. the number
 *            of subsets of {1..n} that form a finite arithmetic progression
 *            including the empty set)
 * Range:     u8 n <= 14, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the offset n = 0 */
    if (n == 0) return 1;

    /* Cached partial sums:
       pre[i] = Σ_{j=1}^{i} Σ_{d=1}^{j} ⌊j/d⌋ ,   pre[0] = 0  */
    static seq_t *pre = NULL;     /* dynamic array of pre sums      */
    static idx_t  cap = 0;        /* current capacity (size of pre) */

    /* Initialise and compute up to index n-1 if needed */
    if (cap == 0) {
        /* start with capacity 1 to store pre[0] */
        pre = (seq_t *)malloc(sizeof(seq_t));
        pre[0] = 0;
        cap = 1;
    }

    if (cap <= n) {                          /* need indices 0 .. n-1   */
        seq_t *new_pre = (seq_t *)realloc(pre, n * sizeof(seq_t));
        if (!new_pre) return 0;              /* allocation failure      */
        pre = new_pre;

        for (idx_t i = cap; i < n; ++i) {     /* compute new entries     */
            idx_t y = i;                      /* current i (i.e., j)    */
            seq_t r = 0;

            /* Efficient calculation of Σ_{d=1}^{y} ⌊y/d⌋   */
            for (idx_t d = 1; d <= y; ) {
                idx_t q    = y / d;           /* quotient ⌊y/d⌋          */
                idx_t next = y / q;           /* last d with this q      */
                idx_t cnt  = next - d + 1;    /* how many d give q       */
                r += (seq_t)q * (seq_t)cnt;   /* contribution to sum     */
                d = next + 1;
            }

            pre[i] = pre[i - 1] + r;          /* cumulative form         */
        }

        cap = n;                              /* now have indices 0..n-1 */
    }

    /* Final answer: 1 (empty) + n (singletons) + pre[n-1] */
    seq_t result = 1;
    result += (seq_t)n;
    result += pre[n - 1];
    return result;
}
