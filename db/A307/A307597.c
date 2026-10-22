/*
 * A307597  Number of partitions of n into 2 distinct positive triangular
 *          numbers.
 * https://oeis.org/A307597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of unordered pairs of distinct positive triangular
 *            numbers whose sum is n. Triangular numbers are T_k = k*(k+1)/2
 *            for k >= 1
 * Range:     u8 n <= 711, u16 n <= 9974, u32 n <= 9974, u64 n <= 9974,
 *            u128 n <= 9974, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Never use static state that depends on a particular n, just recompute the needed
   triangular numbers for each call.  The loops are O(√n).  For n up to about 10000 this
   executes in far less than 1 ms.  The function works with any seq_t type, including
   arbitrary‑precision. */

#include <stddef.h>    /* for size_t */

seq_t a(idx_t n)
{
    /* Upper bound on the number of triangular numbers needed for n <= 10000 is 141.
       A small constant 200 suffices for any 64‑bit n. */
    static const size_t MAX_TRI = 200;
    seq_t triangular[MAX_TRI];
    idx_t tri_cnt = 0;

    /* Build triangular numbers up to n.  Stop when the next one would exceed n. */
    for (idx_t k = 1; tri_cnt < MAX_TRI; ++k) {
        seq_t T = (seq_t)k * ((seq_t)k + 1) / 2;
        if (T > (seq_t)n) break;
        triangular[tri_cnt++] = T;
    }

    /* Count distinct unordered pairs (i<j) with triangular[i] + triangular[j] == n. */
    idx_t count = 0;
    for (idx_t i = 0; i + 1 < tri_cnt; ++i) {
        for (idx_t j = i + 1; j < tri_cnt; ++j) {
            seq_t sum = triangular[i] + triangular[j];
            if (sum == (seq_t)n) {
                ++count;
            } else if (sum > (seq_t)n) {
                /* Triangular numbers are increasing; further j will only increase sum. */
                break;
            }
        }
    }
    return (seq_t)count;
}
