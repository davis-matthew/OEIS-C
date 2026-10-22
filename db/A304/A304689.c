/*
 * A304689  Number of nonnegative integers k such that n - F(k)*F(k+1) is
 *          positive and squarefree, where F(k) denotes the k-th Fibonacci
 *          number A000045(k).
 * https://oeis.org/A304689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k ≥ 0 such that n - F(k)*F(k+1) > 0 and n -
 *            F(k)*F(k+1) is squarefree, where F is the Fibonacci sequence
 * Range:     u8 n <= 687, u16 n <= 8968, u32 n <= 8968, u64 n <= 8968,
 *            u128 n <= 8968, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_squarefree.h"

/* External linkage as required. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset starts at 1 */

    seq_t n_seq = (seq_t)n;                /* convert index to seq_t */
    seq_t f0 = 0;                          /* F(0)   */
    seq_t f1 = 1;                          /* F(1)   */
    int cnt = 0;                           /* result counter */

    while (1) {
        seq_t fk  = f0;                    /* F(k)   */
        seq_t fk1 = f1;                    /* F(k+1) */

        /* If fk and fk1 are non‑zero, check if their product would
           overflow n_seq; avoid multiplication in that case. */
        if (fk != 0 && fk1 != 0) {
            if (fk > n_seq / fk1) break;   /* product > n, stop */
        }

        seq_t prod = fk * fk1;              /* safe: product ≤ n_seq */

        if (prod >= n_seq) break;          /* n - prod ≤ 0, stop */

        seq_t diff = n_seq - prod;          /* positive candidate */

        if (is_squarefree(diff)) cnt++;     /* count if squarefree */

        /* Next Fibonacci numbers */
        seq_t next = f0 + f1;
        f0 = f1;
        f1 = next;
    }

    return (seq_t)cnt;
}
