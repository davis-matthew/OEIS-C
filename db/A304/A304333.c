/*
 * A304333  Number of positive integers k such that n - L(k) is a positive
 *          squarefree number, where L(k) denotes the k-th Lucas number
 *          A000204(k).
 * https://oeis.org/A304333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ k ≥ 1 : n - L(k) is a positive squarefree number }
 *            where L(1)=1, L(2)=3 and L(k)=L(k-1)+L(k-2)
 * Range:     u8 n <= 1316, u16 n <= 8948, u32 n <= 8948, u64 n <= 8948,
 *            u128 n <= 8948, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;               /* the only positive k is none for n=1 */
    seq_t count = 0;

    /* Lucas numbers with L(1)=1, L(2)=3 */
    idx_t l_prev = 1;   /* current Lucas number L(k) */
    idx_t l_next  = 3;   /* next Lucas number L(k+1) */

    while (l_prev < n) {
        idx_t cur = l_prev;
        idx_t diff = n - cur;
        if (is_squarefree((seq_t)diff))
            count++;

        /* advance to next Lucas number */
        idx_t new_next = l_prev + l_next;
        l_prev = l_next;
        l_next = new_next;
    }

    return count;
}
