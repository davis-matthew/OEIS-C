/*
 * A304331  Number of integers k > 1 such that n - F(k) is a positive
 *          squarefree number, where F(k) denotes the k-th Fibonacci number
 *          A000045(k).
 * https://oeis.org/A304331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integers k>1 such that n - F(k) is a positive
 *            squarefree number, where F(k) denotes the k‑th Fibonacci number
 * Range:     u8 n <= 372, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    seq_t count = 0;

    /* Fibonacci generation: F(1)=1, F(2)=1 */
    seq_t f_prev = (seq_t)1;   /* F(1) */
    seq_t f_curr = (seq_t)1;   /* F(2) */
    for (idx_t k = 2; ; ++k)
    {
        /* Stop when F(k) >= n, because then n - F(k) cannot be positive */
        if (f_curr >= (seq_t)n)
            break;

        seq_t diff = (seq_t)n - f_curr;
        if (is_squarefree(diff))
            ++count;

        /* Next Fibonacci number */
        seq_t f_next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = f_next;
    }

    return count;
}
