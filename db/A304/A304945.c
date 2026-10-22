/*
 * A304945  Number of nonnegative integers k such that n - k*L(k) is positive
 *          and squarefree, where L(k) denotes the k-th Lucas number
 *          A000032(k).
 * https://oeis.org/A304945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of nonnegative integers k such that n - k*L(k) is
 *            positive and squarefree, where L(k) is the k-th Lucas number,
 *            OEIS A000032
 * Range:     u8 n <= 232, u16 n <= 9848, u32 n <= 9848, u64 n <= 9848,
 *            u128 n <= 9848, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Potentially used utilities */
#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;                /* offset 1, but guard against n==0 */

    seq_t sn = (seq_t)n;                 /* work in seq_t for all arithmetic */

    /* Lucas numbers: L(0)=2, L(1)=1, L(k)=L(k-1)+L(k-2) for k>=2 */
    seq_t Lprev = 2;                     /* L(0)   */
    seq_t Lcurr = 1;                     /* L(1)   */
    idx_t k = 0;

    seq_t count = 0;                     /* result accumulator */

    for (;;) {
        seq_t Lk;
        if (k == 0)            Lk = Lprev;
        else if (k == 1)       Lk = Lcurr;
        else {                 /* compute next Lucas number */
            Lk = Lprev + Lcurr;
            Lprev = Lcurr;
            Lcurr = Lk;
        }

        seq_t prod = (seq_t)k * Lk;     /* k * L(k) */

        if (prod >= sn) break;          /* n - prod <= 0, stop */

        seq_t m = sn - prod;
        if (m > 0 && is_squarefree(m)) count++;
        k++;
    }

    return count;
}
