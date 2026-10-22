/*
 * A304523  Number of ordered ways to write n as the sum of a Lucas number
 *          (A000032) and a positive odd squarefree number.
 * https://oeis.org/A304523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered pairs (k,m) with n = L_k + m, where L_k
 *            is a Lucas number (L_0 = 2, L_1 = 1) and m is a positive odd
 *            square‑free integer
 * Range:     u8 n <= 548, u16 n <= 9681, u32 n <= 9681, u64 n <= 9681,
 *            u128 n <= 9681, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    /* n is 1‑based; the sequence is defined for n >= 1 */
    if (n < 2) {
        /* a(1) = 0, a(0) is irrelevant */
        return (seq_t)0;
    }

    /* Convert to seq_t for arithmetic */
    seq_t nn = (seq_t)n;

    seq_t count = 0;

    /* Lucas numbers: L0 = 2, L1 = 1 */
    const seq_t l0 = (seq_t)2;
    const seq_t l1 = (seq_t)1;

    /* Check the two initial Lucas numbers explicitly */
    if (l0 < nn) {
        uint64_t m = (uint64_t)(nn - l0);
        if ((m & 1ULL) && is_squarefree(m))
            ++count;
    }
    if (l1 < nn) {
        uint64_t m = (uint64_t)(nn - l1);
        if ((m & 1ULL) && is_squarefree(m))
            ++count;
    }

    /* Generate subsequent Lucas numbers (L2, L3, …) */
    seq_t prev2 = l0; /* L_{k-2} */
    seq_t prev1 = l1; /* L_{k-1} */
    while (1) {
        seq_t next = prev2 + prev1;            /* L_k = L_{k-1} + L_{k-2} */
        if (next >= nn)
            break;                             /* stop when Lucas >= n */

        uint64_t m = (uint64_t)(nn - next);
        if ((m & 1ULL) && is_squarefree(m))
            ++count;

        /* Shift for next iteration */
        prev2 = prev1;
        prev1 = next;
    }

    return count;
}
