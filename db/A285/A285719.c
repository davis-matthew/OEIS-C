/*
 * A285719  a(1) = 1, and for n > 1, a(n) = the largest squarefree number k
 *          such that n-k is also squarefree.
 * https://oeis.org/A285719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest squarefree k <= n-1 such that n-k is also
 *            squarefree
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    idx_t k;
    for (k = n - 1; k > 0; --k) {
        seq_t sk = (seq_t)k;
        seq_t sj = (seq_t)(n - k);
        if (is_squarefree(sk) && is_squarefree(sj))
            return sk;
    }
    /* Should never be reached because at least k=1 works for n>1 */
    return 0;
}
