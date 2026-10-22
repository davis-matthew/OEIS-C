/*
 * A319891  a(n) = 8*7*6*5*4*3*2*1 - 16*15*14*13*12*11*10*9 +
 *          24*23*22*21*20*19*18*17 - ... + (up to the n-th term).
 * https://oeis.org/A319891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = \sum_{k=1}^{n} (-1)^{k-1} * \prod_{i=0}^{7} (8k-i) *
 *            which expands to the equivalent piecewise form given in the OEIS
 *            * comments for k=8. The following implementation follows that *
 *            closed‑form: first term (if n mod 8≠0) and the sum over full
 *            8‑blocks
 * Range:     i8 n <= 2, i16 n <= 6, i32 n <= 23, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   falling_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/falling_product.h"

seq_t a(idx_t n)
{
    const idx_t k = 8;
    if (n < 1) return 0;    /* offset is 1, behaviour for n<1 not required */
    idx_t m = n / k;   /* floor(n/k)    */
    idx_t r = n % k;   /* n mod        */

    seq_t sum = 0;

    /* First term: only j = r (if r≠0) contributes */
    if (r != 0) {
        seq_t base = (seq_t)n + k - r;
        seq_t prod = falling_product(base, r);
        int sign = (m % 2 == 0) ? 1 : -1;   /* (-1)^floor(n/k) */
        sum += (seq_t)sign * prod;
    }

    /* Second term: sum over full 8‑blocks (j = m2*k) */
    for (idx_t m2 = 1; m2 <= m; ++m2) {
        idx_t j = m2 * k;
        seq_t prod = falling_product((seq_t)j, k);
        int sign = (m2 % 2 == 1) ? 1 : -1;   /* (-1)^(m2+1)   */
        sum += (seq_t)sign * prod;
    }

    return sum;
}
