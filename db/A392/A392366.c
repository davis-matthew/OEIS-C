/*
 * A392366  The odd squarefree numbers squared.
 * https://oeis.org/A392366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 105, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = the nth odd squarefree number, squared. */
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for offset 1, but defensive */

    seq_t odd = 1;          /* first odd integer */
    idx_t count = 0;        /* number of odd squarefree numbers seen */

    for (;; odd += 2) {
        if (is_squarefree(odd)) {
            count++;
            if (count == n)
                return odd * odd;   /* square of the nth odd squarefree */
        }
    }
}
