/*
 * A350134  Number of endofunctions on [n] with at least one isolated fixed
 *          point.
 * https://oeis.org/A350134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (-1)^{k+1} * C(n,k) * (n-k)^(n-k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of endofunctions on [n] with at least one isolated fixed point */

#include <stdlib.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;

    seq_t pos = 0, neg = 0;
    seq_t binom = 1;                       // C(n,0)
    for (idx_t k = 1; k <= n; ++k) {
        /* Update binomial coefficient: C(n,k) = C(n,k-1)*(n-k+1)/k */
        binom = (binom * (seq_t)(n - k + 1)) / (seq_t)k;

        idx_t b = n - k;
        seq_t powv;
        if (b == 0)
            powv = 1;                       /* 0^0 is defined as 1 for this sum */
        else
            powv = big_pow((seq_t)b, (seq_t)b);

        seq_t term = binom * powv;
        if (k & 1) /* k odd → positive term */
            pos += term;
        else       /* k even → negative term */
            neg += term;
    }
    return pos - neg;                       /* pos >= neg for all n */
}
