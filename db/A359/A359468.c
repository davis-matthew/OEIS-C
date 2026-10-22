/*
 * A359468  Numbers that are either multiples of 4 with their odd part
 *          squarefree, or that are not multiples of 4 and not squarefree.
 * https://oeis.org/A359468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer k such that (k is a multiple of
 *            4 and its odd part is squarefree) or (k is not a multiple of 4
 *            and k itself is not squarefree)
 * Range:     u8 n <= 88, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A359468: numbers that are multiples of 4 with their odd part squarefree,
    or that are not multiples of 4 and not squarefree. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_squarefree.h"   /* bool is_squarefree(seq_t x); */

/* Compute the nth term of A359468.  The sequence starts at n = 1
   (a(1) = 4).  For n == 0 we return 0 for safety, though that value
   is outside the defined range. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* offset is 1 – undefined for n==0 */

    seq_t k = 0;                   /* candidate numbers start from 1 */
    idx_t cnt = 0;                 /* how many terms have been found */

    while (cnt < n) {
        ++k;                       /* next integer */

        bool ok;
        if ((k & 3) == 0) {        /* multiple of 4? */
            seq_t odd = k;
            while ((odd & 1) == 0) odd >>= 1;   /* remove all factors of 2 */
            ok = is_squarefree(odd);            /* odd part must be squarefree */
        } else {                   /* not a multiple of 4 */
            ok = !is_squarefree(k);              /* k must not be squarefree */
        }

        if (ok)
            ++cnt;
    }

    return k;                     /* the nth term */
}
